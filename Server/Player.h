#pragma once

#include "Object.h"
#include "../Define.h"
#include "Field.h"
#include "Server.h"

#include <string>

class Player : public Object {
	public:
        Player(const Field & _field, const std::string _name);
        void MakeMovement(const Coordinate & coordinate, enum Event);
		void PutBomb();
		void GetDamage();
		void Heal();
		int GetHp();
		int GetBomb();
        std::string GetName();
	private:
        Coordinate PositionAfterMovement(const Coordinate & coordinate, enum Event);
        int ToVectorCoordinate(const Coordinate & coordinate);

		std::string name;
		int bomb;
		int hp;
		int skin;
        const Field & field;
};