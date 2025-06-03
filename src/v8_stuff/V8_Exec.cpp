#include "V8_Exec.h"

// ActiveTab activeTab{false, "NewDocument1", 1, "Example file"};

void V8_Exec::logCallback(const v8::FunctionCallbackInfo<v8::Value>& info) 
{
    if (info.Length() < 1) return;
    v8::Isolate* isolate = info.GetIsolate();
    v8::HandleScope scope(isolate);
    v8::Local<v8::External> data = info.Data().As<v8::External>();
    V8_Exec* self = static_cast<V8_Exec*>(data->Value());
    std::string message;
    for (int i=0; i<info.Length();i++){
        v8::String::Utf8Value str(isolate, info[i]);
        message += *str;
        if (i < info.Length()-1){
            message += " ";
        }
    }
    // if (self->application) {
    //     QString param = QString::fromStdString("hello from v8");
    //     QString result;

    //     bool invoked = QMetaObject::invokeMethod(
    //         self->application,
    //         "cb",
    //         Qt::BlockingQueuedConnection,
    //         Q_RETURN_ARG(QString, result),
    //         Q_ARG(QString, param)
    //     );
    //     qDebug() << "response from app:" << result;
    // }
    self->appendToOutput(message);
}

void V8_Exec::getAllTabs(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = args.GetIsolate();
    v8::HandleScope scope(isolate);
    v8::Local<v8::External> data = args.Data().As<v8::External>();
    V8_Exec* self = static_cast<V8_Exec*>(data->Value());
    
    std::vector<ActiveTab> openTabs;
    bool invoked = QMetaObject::invokeMethod(
        self->application,
        "getAllOpenTabs",
        Qt::BlockingQueuedConnection,
        Q_RETURN_ARG(std::vector<ActiveTab>, openTabs)
    );
    self->openTabs = std::move(openTabs);
    if (!invoked) {
        args.GetReturnValue().Set(v8::Undefined(isolate));
        return;
    }
    v8::Local<v8::Array> jsArray = v8::Array::New(isolate, static_cast<int>(self->openTabs.size()));
    
    for (size_t i=0; i<self->openTabs.size();i++) {
        ActiveTab* tabPtr = &self->openTabs[i];
        v8::Local<v8::Object> jsTab = createActiveTabObject(
            self->isolate,
            self->isolate->GetCurrentContext(),
            self->activeTabTemplate,
            tabPtr
        );
        jsArray->Set(isolate->GetCurrentContext(), static_cast<uint32_t>(i), jsTab).Check();
    }
    args.GetReturnValue().Set(jsArray);
}

V8_Exec::V8_Exec(int argc, char** argv, Application* application) : argc(argc), argv(argv), application(application)
{
    qDebug()<<"V8_Exec constructor";
}

void V8_Exec::initializeV8()
{
    qDebug()<<"initializeV8";
    v8::V8::InitializeICUDefaultLocation(argv[0]);
    v8::V8::InitializeExternalStartupData(argv[0]);
    platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::SetFlagsFromCommandLine(&argc, argv, true);
    v8::V8::Initialize();
    this->create_params.array_buffer_allocator =
        v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    isolate = v8::Isolate::New(create_params);
    v8::HandleScope scope(isolate);
    v8::Local<v8::ObjectTemplate> localTemplate = v8::ObjectTemplate::New(isolate);
    localTemplate->SetInternalFieldCount(1);
    this->activeTabTemplate.Reset(isolate, localTemplate); // Store as Global
}

v8::Local<v8::ObjectTemplate> V8_Exec::Initialize()
{
    v8::EscapableHandleScope handle_scope(isolate);
    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
    global->Set(
        v8::String::NewFromUtf8Literal(isolate, "log"),
        v8::FunctionTemplate::New(isolate, logCallback, v8::External::New(isolate, this))
    );
    return handle_scope.Escape(global);
}

void V8_Exec::executeScript(const std::string &scriptToExecute)
{
    this->lastOutput = "";
    {
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::ObjectTemplate> global = this->Initialize();
        v8::Local<v8::Context> context = v8::Context::New(isolate, nullptr, global);
        v8::Context::Scope context_scope(context);

        v8::Local<v8::Object> globalObject = context->Global();
        v8::Local<v8::Object> activeTabObject = createActiveTabObject(
            isolate,
            context,
            this->activeTabTemplate,
            &activeTab
        );

        v8::PropertyAttribute attrLocal = static_cast<v8::PropertyAttribute>(
            v8::PropertyAttribute::ReadOnly | v8::PropertyAttribute::DontDelete
        );

        globalObject->DefineOwnProperty(
            context,
            v8::String::NewFromUtf8Literal(isolate, "activeTab"),
            activeTabObject,
            attrLocal
        ).Check();

        v8::Local<v8::Function> nativeFunc = v8::FunctionTemplate::New(
            isolate,
            getAllTabs,
            v8::External::New(isolate, this)
        )->GetFunction(context).ToLocalChecked();

        v8::PropertyAttribute attr = static_cast<v8::PropertyAttribute>(
            v8::ReadOnly | v8::DontDelete
        );

        context->Global()->DefineOwnProperty(
            context,
            v8::String::NewFromUtf8Literal(isolate, "getAllTabs"),
            nativeFunc,
            attr
        ).Check();

        v8::TryCatch trycatch(isolate);
        v8::Local<v8::String> source;

        if (!v8::String::NewFromUtf8(isolate, scriptToExecute.c_str(), v8::NewStringType::kNormal).ToLocal(&source)) {
            qDebug() << "Failed to create V8 string from script";
            return;
        }

        v8::Local<v8::Script> script;
        if (!v8::Script::Compile(context, source).ToLocal(&script)) {
            v8::String::Utf8Value exception(isolate, trycatch.Exception());
            qDebug() << "CompileError:" << *exception;
            std::string resultStr(*exception);
            emit scriptResultReady(resultStr);
            return;
        }

        v8::Local<v8::Value> result;
        if (!script->Run(context).ToLocal(&result)) {
            v8::String::Utf8Value exception(isolate, trycatch.Exception());
            qDebug() << "Run Error:" << *exception;
            std::string resultStr(*exception);
            emit scriptResultReady(resultStr);
            return;
        }

        v8::String::Utf8Value utf8(isolate, result);
        std::string resultStr(lastOutput);
        emit scriptResultReady(resultStr);
        return;
    }
}

V8_Exec::~V8_Exec()
{
    qDebug()<<"desctructor v8";
    activeTabTemplate.Reset();
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::DisposePlatform();
    delete this->create_params.array_buffer_allocator;
}

void V8_Exec::updateActiveTab(ActiveTab tab) {
    qDebug()<<tab.textContent;
    this->activeTab = tab;
}

void V8_Exec::appendToOutput(std::string &str)
{   
    if (lastOutput.empty()) {
        this->lastOutput += str;
    }
    else {
        this->lastOutput += "\n";
        this->lastOutput += str;
    }
}