#include"GameHeaders.h"


int main()
{
	Timeline* tl = new Timeline();
	tl->setTicSize(50);

	// ScriptManager initialization code
	std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.release());
    v8::V8::InitializeICU();
    v8::V8::Initialize();
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    v8::Isolate *isolate = v8::Isolate::New(create_params);
	
	v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
    v8::HandleScope handle_scope(isolate);

	// Best practice to install all global functions in the context ahead of time.
    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
    // Bind the global 'print' function to the C++ Print callback.
    global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, v8helpers::Print));
	// Bind the global static function for retrieving object handles
	global->Set(isolate, "gethandle", v8::FunctionTemplate::New(isolate, ScriptManager::getHandleFromScript));
    v8::Local<v8::Context> default_context =  v8::Context::New(isolate, NULL, global);
	v8::Context::Scope default_context_scope(default_context); // enter the context

    ScriptManager *sm = new ScriptManager(isolate, default_context); 

	Game game(tl, sm, isolate, default_context);

	while (game.getWindow().isOpen()) {		
		if (!tl->Paused) {
			game.update();
			game.render();
		}
		tl->updateTime();
		std::this_thread::sleep_for(std::chrono::milliseconds(tl->getTimeRemainingTic()));
	}
	//End of application
	isolate->Dispose();
	delete sm;
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
	delete tl;
	return 0;
}