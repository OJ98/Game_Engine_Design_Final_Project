#pragma once

class GameObject : public sf::RectangleShape{
public:
	// Constructors & Destructors
	GameObject();
	virtual ~GameObject();

	std::string guid;

	nlohmann::json ScriptEvent;

	v8::Local<v8::Object> exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name="default");

	static std::vector<GameObject*> game_objects;

private:

	/** Static variable to keep count of current number of GameObject instances */
	static int current_guid;

	v8::Isolate* isolate;
	v8::Global<v8::Context>* context;

	static void setGameObjectGUID(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
	static void getGameObjectGUID(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info); // note return type

	static void setGameObjectJSON(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);
	static void getGameObjectJSON(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info); // note return type

};