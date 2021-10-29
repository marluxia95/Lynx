require "LynxCore"

function CustomEvent(event) 
    LynxCore.log_info("You pressed key " .. event.KeyCode)
end

LynxCore.AddEventListener("KeyPressed", CustomEvent)