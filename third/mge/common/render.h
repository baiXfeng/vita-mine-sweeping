//
// Created by baifeng on 2021/7/12.
//

#ifndef SDL2_UI_RENDER_H
#define SDL2_UI_RENDER_H

#include <SDL.h>
#include <memory>
#include "macro.h"
#include "vector2.h"

mge_begin

class DrawColor {
public:
    DrawColor(SDL_Renderer* r):_renderer(r) {
        SDL_GetRenderDrawColor(_renderer, &_back.r, &_back.g, &_back.b, &_back.a);
    }
    ~DrawColor() {
        SDL_SetRenderDrawColor(_renderer, _back.r, _back.g, _back.b, _back.a);
    }
    void setColor(SDL_Color const& c) {
        SDL_SetRenderDrawColor(_renderer, c.r, c.g, c.b, c.a);
    }
private:
    SDL_Renderer* _renderer;
    SDL_Color _back;
};

class TextureColor {
public:
    TextureColor(SDL_Texture* texture):_texture(texture) {
        SDL_GetTextureColorMod(_texture, &_back.r, &_back.g, &_back.b);
        SDL_GetTextureAlphaMod(_texture, &_back.a);
    }
    ~TextureColor() {
        SDL_SetTextureColorMod(_texture, _back.r, _back.g, _back.b);
        SDL_SetTextureAlphaMod(_texture, _back.a);
    }
    void setColor(SDL_Color const& c) {
        SDL_SetTextureColorMod(_texture, c.r, c.g, c.b);
        SDL_SetTextureAlphaMod(_texture, c.a);
    }
private:
    SDL_Texture* _texture;
    SDL_Color _back;
};

class Texture;
class RenderCopy {
public:
    typedef std::shared_ptr<Texture> TexturePtr;
public:
    RenderCopy();
    virtual ~RenderCopy() {}
public:
    TexturePtr& texture();
    TexturePtr const& texture() const;
    void setTexture(TexturePtr const& texture);
    void setTexture(TexturePtr const& texture, SDL_Rect const& srcrect);
    void setSize(int w, int h);
    void setSize(Vector2i const& size);
    Vector2i const& size() const;
    void setOpacity(int opacity);
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    int opacity() const;
    SDL_Color const& color() const;
public:
    virtual void draw(SDL_Renderer* renderer, Vector2i const& position = {0, 0});
protected:
    TexturePtr _texture;
    SDL_Rect _srcrect;
    SDL_Color _color;
    Vector2i _size;
};

class RenderCopyEx : public RenderCopy {
public:
    RenderCopyEx();
public:
    void setScale(Vector2f const& scale);
    Vector2f const& getScale() const;
    void setAnchor(Vector2f const& anchor);
    Vector2f const& getAnchor() const;
    void setAngle(float angle);
    float getAngle() const;
public:
    void draw(SDL_Renderer* renderer, Vector2i const& position = {0, 0}) override;
protected:
    float _angle;
    Vector2f _scale;
    Vector2f _anchor;
};

class RenderFillRect {
public:
    RenderFillRect();
    virtual ~RenderFillRect() {}
public:
    void setSize(int w, int h);
    Vector2i const& size() const;
    void setColor(SDL_Color const& c);
    SDL_Color const& color() const;
public:
    virtual void draw(SDL_Renderer* renderer, Vector2i const& position = {0, 0});
protected:
    Vector2i _size;
    SDL_Color _color;
};

class RenderDrawRect : public RenderFillRect {
public:
    void draw(SDL_Renderer* renderer, Vector2i const& position = {0, 0}) override;
};

mge_end

#endif //SDL2_UI_RENDER_H
