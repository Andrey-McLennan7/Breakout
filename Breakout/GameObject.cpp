#include "GameObject.h"

GameObject::GameObject(SDL_Renderer* _renderer, std::string _file,
    int _x, int _y, int _w, int _h, float _speed) :

    m_speed{ _speed },

    Sprite{ _renderer, _file, _x, _y, _w, _h }
{}

GameObject::~GameObject()
{}

bool GameObject::CheckCollision(const GameObject* _obj)
{
    if (
        // this object's right collides with other object's left
        this->m_position.x < (_obj->m_position.x + _obj->m_position.w) &&

        // this object's left collides with other object's right
        (this->m_position.x + this->m_position.w) > _obj->m_position.x &&

        // this object's top collides with other object's bottom
        this->m_position.y < (_obj->m_position.y + _obj->m_position.h) &&

        // this object's bottom collides with other object's top
        (this->m_position.y + this->m_position.h) > _obj->m_position.y
       )
    {
        return true;
    }

    return false;
}