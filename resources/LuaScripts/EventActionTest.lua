-- Test doc for demonstrating how to use EventActions in lua and in C++

-- creation and assignment is done like this
fireball = FireballEvent(100)

-- methods in classes are called with a colon
fireball:call()

-- I bound printEventAction as a test for showing how to use them in C++. If this works the action will be called and the type printed in C++.
printEventAction(fireball)


-- Now to demonstrate the CustomEvent
function doom() print("DOOOOMMM!!!") end
customAction = CustomEvent(doom)

-- if this works DOOOOMMM!!! will be printed in lua
customAction:call()

-- This time from C++. Again, the call() method will be called and the type will be print in C++.
printEventAction(customAction)
