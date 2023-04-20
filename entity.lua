Entity = {}

function Entity:new(vertex, fragment, texture)
    local object = {
        mesh     = engine.create_mesh(),
        vertex   = vertex,
        fragment = fragment,
        texture  = texture
    }

    setmetatable(object, {__index = Entity})

    function Entity:initialization()
        engine.create_shader_program(self.mesh, self.vertex, self.fragment)
        engine.create_texture       (self.mesh, self.texture)
    end

    object:initialization()

    return object
end

function Entity:set_scale(scale) engine.set_scale(self.mesh, scale) end

function Entity:set_position(position) engine.set_translation(self.mesh, position.x, position.y) end

function Entity:draw() engine.draw(self.mesh) end
