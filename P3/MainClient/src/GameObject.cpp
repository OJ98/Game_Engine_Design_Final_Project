#include"GameHeaders.h"

/** Definitions of static class members */
int GameObject::current_guid = 0;
std::vector<GameObject*> GameObject::game_objects;

// Constructor
GameObject::GameObject() {
    guid = "gameobject" + std::to_string(current_guid);
	current_guid++;
	game_objects.push_back(this);
	this->ScriptEvent["Event_Type"] = "None";
	this->ScriptEvent["Raised"] = false;
}

// Destructor
GameObject::~GameObject() {
    context->Reset();
}


v8::Local<v8::Object> GameObject::exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name)
{
	std::vector<v8helpers::ParamContainer<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> v;
	v.push_back(v8helpers::ParamContainer("event_json", getGameObjectJSON, setGameObjectJSON));
	v.push_back(v8helpers::ParamContainer("guid", getGameObjectGUID, setGameObjectGUID));
	return v8helpers::exposeToV8(guid, this, v, isolate, context, context_name);
}

void GameObject::setGameObjectGUID(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	v8::String::Utf8Value utf8_str(info.GetIsolate(), value->ToString());
	static_cast<GameObject*>(ptr)->guid = *utf8_str;
}

void GameObject::getGameObjectGUID(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	std::string guid = static_cast<GameObject*>(ptr)->guid;
	v8::Local<v8::String> v8_guid = v8::String::NewFromUtf8(info.GetIsolate(), guid.c_str(), v8::String::kNormalString);
	info.GetReturnValue().Set(v8_guid);
}

void GameObject::setGameObjectJSON(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	v8::String::Utf8Value utf8_str(info.GetIsolate(), value->ToString());
	static_cast<GameObject*>(ptr)->ScriptEvent = json::parse(*utf8_str);
}

void GameObject::getGameObjectJSON(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();
	std::string event = static_cast<GameObject*>(ptr)->ScriptEvent.dump();
	v8::Local<v8::String> ev = v8::String::NewFromUtf8(info.GetIsolate(), event.c_str(), v8::String::kNormalString);
	info.GetReturnValue().Set(ev);
}
