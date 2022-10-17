.data
    a: .byte 15,33,32,6,36,51,57,17,33,23,14,6,14,43,10,6,14,31,25,36,21,30,37,62,18,15,20,54,12,27,33,63,0,58,54,36,54,48,56,16,48,48,5,8,62,30,3,42,0,63;
    b: .byte 19,1,19,53,45,16,18,7,51,28,52,63,54,38,48,17,31,45,10,17,15,31,53,1,38,56,47,38,7,62,28,42,6,57,47,51,29,17,50,43,52,54,45,8,54,52,6,4,1,63;
    c: .space 50
    min: .space 1
    max: .space 1

.text

main:
    daddui R1, R0, 50        #counter for cycle
    dadd R2, R0, R0         #multiplier for vector address

loop1:
    lb R3, a(R2)            #load in R3 a[i]
    lb R4, b(R2)            #load in R4 b[i]

    dadd R3, R3, R4         #R3 = a[i] + b[i]

    sb R3, c(R2)            #c[i] = R3 = a[i] + b[i]

    daddi R1, R1, -1        #decrement loop counter
    daddi R2, R2, 1         #increment vector address multiplier

    bnez R1, loop1

    lb R4, c(R0)            #R4 --> min
    lb R5, c(R0)            #R5 --> max

#now R2 is the counter for the cycle and R1 is the vector multiplier
loop2:
    lb R3, c(R1)            #load value of vector to check

    slt R6, R3, R4          #R3 < R4 (new value R3 is less than current minimun R4 --> R3 new minimum)
    movn R4, R3, R6         #move R3 (new value) in current minimum R4

    slt R6, R5, R3          #R5 < R3 (current maximum R5 is less than new value R3 --> R3 new maximun)
    movn R5, R3, R6         #move R3 (new value) in current maximum R5

    daddi R2, R2, -1        #decrement counter
    daddi R1, R1, 1         #increment vector address multiplier

    bnez R2, loop2

    sb R4, min(R0)
    sb R5, max(R0)

    halt