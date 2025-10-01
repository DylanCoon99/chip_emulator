
with open("./chip48font.txt", "r") as f:
    lines = f.readlines()
    
    data = ""
    
    for line in lines:
        newLine = line[2:].strip('\n')
        
        d = str(hex(int(newLine, 2)))[2:]
        
    
        data += d
        #print(type(line), line)
        
    print(data)
