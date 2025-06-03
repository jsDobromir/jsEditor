#ifndef ACTIVE_TAB
#define ACTIVE_TAB
#include <v8.h>
#include "../../application/ActiveTab.h"
void isFileCallback(const v8::FunctionCallbackInfo<v8::Value>& args);
void getName(const v8::FunctionCallbackInfo<v8::Value>& args);
void getText(const v8::FunctionCallbackInfo<v8::Value>& args);

v8::Local<v8::Object> createActiveTabObject(
    v8::Isolate* isolate,
    v8::Local<v8::Context> context,
    v8::Global<v8::ObjectTemplate>& templateRef,
    ActiveTab* tab
);

#endif