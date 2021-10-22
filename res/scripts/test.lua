require "LynxCore"

function CustomEvent(event) 

    LynxCore.log_info("You pressed key " .. event.KeyCode)

    return
end

function SecondCustomEvent(event) 

    LynxCore.log_info("Halal free dwonload 1 link ")

    return
end

LynxCore.AddEventListener("KeyPressed", CustomEvent)
LynxCore.AddEventListener("LastTick", SecondCustomEvent)
