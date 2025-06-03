#include "activeTab.h"

void isFileCallback(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = args.GetIsolate();
    v8::HandleScope scope(isolate);
    v8::Local<v8::Object> self = args.Holder();
    v8::Local<v8::Data> field = self->GetInternalField(0);
    if (self->InternalFieldCount() == 0) {
        args.GetReturnValue().Set(v8::Undefined(isolate));
        return;
    }
    v8::Local<v8::External> externalField = field.As<v8::External>();
    ActiveTab* tab = static_cast<ActiveTab*>(externalField->Value());
    args.GetReturnValue().Set(v8::Boolean::New(isolate, tab->isFile));
}

void getName(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = args.GetIsolate();
    v8::HandleScope scope(isolate);
    v8::Local<v8::Object> self = args.Holder();
    v8::Local<v8::Data> field = self->GetInternalField(0);
    if (self->InternalFieldCount() == 0) {
        args.GetReturnValue().Set(v8::Undefined(isolate));
        return;
    }
    v8::Local<v8::External> externalField = field.As<v8::External>();
    ActiveTab* tab = static_cast<ActiveTab*>(externalField->Value());
    QByteArray utf8Name = tab->name.toUtf8();
    args.GetReturnValue().Set(
        v8::String::NewFromUtf8(isolate, utf8Name.constData()).ToLocalChecked()
    );
}

void getText(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = args.GetIsolate();
    v8::HandleScope scope(isolate);
    v8::Local<v8::Object> self = args.Holder();
    v8::Local<v8::Data> field = self->GetInternalField(0);
    if (self->InternalFieldCount() == 0) {
        args.GetReturnValue().Set(v8::Undefined(isolate));
        return;
    }
    v8::Local<v8::External> externalField = field.As<v8::External>();
    ActiveTab* tab = static_cast<ActiveTab*>(externalField->Value());
    QByteArray utf8Name = tab->textContent.toUtf8();
    args.GetReturnValue().Set(
        v8::String::NewFromUtf8(isolate, utf8Name.constData()).ToLocalChecked()
    );
}

v8::Local<v8::Object> createActiveTabObject(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Global<v8::ObjectTemplate>& templateRef, ActiveTab* tab)
{
    v8::Local<v8::ObjectTemplate> tmpl = templateRef.Get(isolate);
    v8::Local<v8::Object> obj = tmpl->NewInstance(context).ToLocalChecked();
    obj->SetInternalField(0, v8::External::New(isolate, tab));

    obj->Set(
        context,
        v8::String::NewFromUtf8Literal(isolate, "isFile"),
        v8::FunctionTemplate::New(isolate, isFileCallback)->GetFunction(context).ToLocalChecked()
    ).Check();

    obj->Set(
        context,
        v8::String::NewFromUtf8Literal(isolate, "getName"),
        v8::FunctionTemplate::New(isolate, getName)->GetFunction(context).ToLocalChecked()
    ).Check();

    obj->Set(
        context,
        v8::String::NewFromUtf8Literal(isolate, "getText"),
        v8::FunctionTemplate::New(isolate, getText)->GetFunction(context).ToLocalChecked()
    ).Check();

    return obj;
}