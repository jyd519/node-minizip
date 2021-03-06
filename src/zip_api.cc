// Copyright 2014 Haojian Wu. All rights reserved.
// Use of this source code is governed by MIT license that can be found in the
// LICENSE file.

#include "nan.h"
#include "zip_async_worker.h"

#include <iostream>
using namespace std;

namespace {

NAN_METHOD(Zip) {
  Nan::HandleScope scope;

  if (info.Length() != 3 && info.Length() != 4 && info.Length() != 5){
       Nan::ThrowTypeError("Bad number of arguments");
       return;
  } else if(info.Length() == 3) {
      if(!info[0]->IsString() || !info[1]->IsString() || !info[2]->IsFunction()) {
        Nan::ThrowTypeError("Bad arguments");
        return;
      }

      std::string src_dir(*(v8::String::Utf8Value(info[0])));
      std::string dest_dir(*(v8::String::Utf8Value(info[1])));

      Nan::Callback* callback = new Nan::Callback(info[2].As<v8::Function>());
      Nan::AsyncQueueWorker(new zip::ZipAsyncWorker(src_dir, dest_dir, callback));
  } else if(info.Length() == 4) {
      if(!info[0]->IsString() || !info[1]->IsString() || !info[2]->IsString() || !info[3]->IsFunction()) {
        Nan::ThrowTypeError("Bad arguments");
        return;
      }

      std::string src_dir(*(v8::String::Utf8Value(info[0])));
      std::string dest_dir(*(v8::String::Utf8Value(info[1])));
      std::string password(*(v8::String::Utf8Value(info[1])));

      Nan::Callback* callback = new Nan::Callback(info[3].As<v8::Function>());
      Nan::AsyncQueueWorker(new zip::ZipAsyncWorker(src_dir, dest_dir, callback, password.c_str()));
  } else if(info.Length() == 5) {
      if (!info[0]->IsObject() || !info[1]->IsString() || !info[2]->IsString() || !info[3]->IsString() || !info[4]->IsFunction()){ 
        Nan::ThrowTypeError("Bad arguments");
        return;
      }

      v8::Local<v8::Object> stream = info[0].As<v8::Object>();
      size_t size = node::Buffer::Length(stream);
      void *buffer = node::Buffer::Data(stream);
      std::string relative_path(*(v8::String::Utf8Value(info[1])));
      std::string dest_file(*(v8::String::Utf8Value(info[2])));
      std::string password(*(v8::String::Utf8Value(info[3])));

      Nan::Callback* callback = new Nan::Callback(info[4].As<v8::Function>());
      Nan::AsyncQueueWorker(new zip::ZipAsyncWorker(buffer, size, relative_path, dest_file, callback, password)); 
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

NAN_METHOD(Unzip) {
  Nan::HandleScope scope;

  if (info.Length() != 3 && info.Length() != 4) {
    Nan::ThrowTypeError("Bad arguments");
    return;
  } else if(info.Length() == 3){
     if(!info[0]->IsString() || !info[1]->IsString() || !info[2]->IsFunction()) {
        Nan::ThrowTypeError("Bad arguments");
        return;
     }

     std::string zip_file(*(v8::String::Utf8Value(info[0])));
     std::string dest_dir(*(v8::String::Utf8Value(info[1])));

     Nan::Callback* callback = new Nan::Callback(info[2].As<v8::Function>());

     Nan::AsyncQueueWorker(new zip::UnzipAsyncWorker(zip_file, dest_dir, callback));
  } else if(info.Length() == 4){
     if (!info[0]->IsString() || !info[1]->IsString() || !info[2]->IsString() || !info[3]->IsFunction()) {
       Nan::ThrowTypeError("Bad arguments");
       return;
     }

     std::string zip_file(*(v8::String::Utf8Value(info[0])));
     std::string dest_dir(*(v8::String::Utf8Value(info[1])));
     std::string password(*(v8::String::Utf8Value(info[2])));

     Nan::Callback* callback = new Nan::Callback(info[3].As<v8::Function>());
     Nan::AsyncQueueWorker(new zip::UnzipAsyncWorker(zip_file, dest_dir, callback, password));
  }

  info.GetReturnValue().Set(Nan::Undefined());
}

}  // namespace

NAN_MODULE_INIT(init) {
  Nan::Set(target, Nan::New("zip").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(Zip)->GetFunction());
  Nan::Set(target, Nan::New("unzip").ToLocalChecked(),
                 Nan::New<v8::FunctionTemplate>(Unzip)->GetFunction());
}

NODE_MODULE(node_minizip, init)
