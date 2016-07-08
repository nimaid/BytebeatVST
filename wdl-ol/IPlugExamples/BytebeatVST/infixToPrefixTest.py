def infixToPrefix(infix):
    stack = []
    output = []

    precedence = {'^': 3, '*': 2, '/': 2, '+': 1, '-': 1}

    infix.reverse()

    stack.append(')')

    print(infix)
    for x in infix:
        print("x = " + str(x))
        print("Before: stack = " + str(stack))
        print("Before: output = " + str(output))
        if x not in precedence and x != '(' and x != ')':
            output.append(x)
        elif x == ')':
            stack.append(x)
        elif x in precedence:
            if len(stack) > 0:
                if stack[-1] != ')':
                    while precedence[stack[-1]] >= precedence[x]:
                        temp = stack.pop()
                        
                        if temp != ')':
                            output.append(temp)
                            
                        if len(stack) <= 0:
                            break;
                        
                        if stack[-1] == ')':
                            break;

            stack.append(x)
        elif x == '(':
            if len(stack) > 0:
                while stack[-1] != ')':
                    temp = stack.pop()
                    
                    if temp != ')':
                        output.append(temp)

                    if len(stack) <= 0:
                        break;

        print("After: stack = " + str(stack))
        print("After: output = " + str(output))
        print()

    while len(stack) > 0:
        temp = stack.pop()
        if temp != ')':
            output.append(temp)
    
    output.reverse()

    return output
            
            
