.data
    v1: .byte 1, 2, 3, 4, 5, 6, 7, 8, 7, 6, 5, 4, 3, 2, 1;
    flag: .space 1

.text

main:
    daddui R1, R0, 14       #counter for cycle
    dadd R2, R0, R0         #multiplier for vector address

    daddu R5, R0, R0        #initialize flag at 0

loop:
    lb R3, v1(R2)          #load first byte in R3
    lb R4, v1(R1)          #load last byte in R4

    bne R3, R4, end         #if i byte != (n-i) byte jump to end

    daddi R1, R1, -1        #decrement loop counter
    daddi R2, R2, 1         #increment vector address multiplier

    #vector of even dimension
    slt R6, R2, R1          #if R2 < R2 we are over the half of the vector
    beqz R6, endloop        #jump and set flag to 1 (already jumped to end --> flag = 0 if not palindrome)
    #vector of odd dimension
    beq R1, R2, endloop     #reached half of the vector, don't need to check other half, jump and set flag to 1 (already jumped to end --> flag = 0 if not palindrome)

    j loop                  #keep loop going

endloop:
    daddui R5, R0, 1        #set flag to 1

end:
    sb R5, flag(R0)
    halt