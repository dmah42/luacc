globalVar1 = 0  -- let's include a comment here

--[[
        nothing like block comments as a good
        test of a lexer
]]

function func1(param1, param2)
        return param1 + param2
end

function func2()
        local localVar1 = func1(1, 2)

        globalVar2 = localVar1
end

