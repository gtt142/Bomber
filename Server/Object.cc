#include "Object.h"

Object::Object(): position(0, 0) {
    SetId();
}

Object::Object(Coordinate startPosition): position(startPosition) {
	SetId();
}

void Object::SetId() {
    static int globalId = 0;
    id = globalId++;
}

int Object::GetId() {
	return id;
}

Coordinate Object::GetPosition() {
	return position;
}

void Object::SetPosition(Coordinate _position) {
    position = _position;
}