require "io"
require "engine"
require "entity"

function read(file_path)
    local file = io.open(file_path, "r")
    local code = ""

    if file then
        code = file:read("*all")

        file:close()
    else
        print "Erro ao abrir arquivo!"
    end

    return code
end

local KEYS = {
    W = 87,
    A = 65,
    S = 83,
    D = 68
}

function update(window, hero, position, speed)
    if (engine.get_key(window, KEYS.W)) then position.y = position.y + speed end
    if (engine.get_key(window, KEYS.A)) then position.x = position.x - speed end
    if (engine.get_key(window, KEYS.S)) then position.y = position.y - speed end
    if (engine.get_key(window, KEYS.D)) then position.x = position.x + speed end

    hero:set_position(position)
end

local VERSION_MAJOR = 3
local VERSION_MINOR = 3

engine.initialization(VERSION_MAJOR, VERSION_MINOR)

local WIDTH  = 600
local HEIGHT = 400

local window = engine.create_window("Game", WIDTH, HEIGHT)

engine.set_window_icon   (window, "./img/window_icon.bmp")
engine.activate_extension()

local vertex   = read("./glsl/vertex.glsl")
local fragment = read("./glsl/fragment.glsl")
local hero     = Entity:new(vertex, fragment, "./img/hero.bmp")
local position = {x = 0.25, y = 0.5}

hero:set_scale   (0.25)
hero:set_position(position)

engine.set_aspect(WIDTH / HEIGHT)

local FPS           = 60
local last_frame    = 0.0
local current_frame = 0.0

while not engine.window_should_close(window) do
    current_frame = engine.get_time()

    if (current_frame - last_frame) > (1.0 / FPS) then
        engine.clear_color(0.5, 0.5, 1.0)

        update   (window, hero, position, 0.05)
        hero:draw()

        engine.swap_buffers(window)
        engine.poll_events ()

        last_frame = current_frame
    end
end

engine.destroy_window(window)
engine.terminate     ()
