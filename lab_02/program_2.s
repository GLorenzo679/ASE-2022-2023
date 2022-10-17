.data
ifmap: 		.byte	0,0,0,0,0,0,1,2,3,0,0,4,5,6,0,0,7,8,9,0,0,0,0,0,0;
kernel: 	.byte	0,-1,-1,0,2,1,1,0,4;
ofmap:		.space   9

.text

main:
    daddui R1, R0, 5        #dim row ifmap
    daddui R2, R0, 3        #dim row kernel
    daddui R17, R0, 3       #dim row ofmap
    daddui R3, R0, 0        #ifmap start
    daddui R10, R0, 9       #dim kernel matrix

    daddui R4, R0, 0        #x start position
    daddui R5, R0, 0        #y start position

    daddui R6, R0, 0        #ofmap index
    daddui R7, R0, 0        #kernel index
    daddui R8, R0, 0        #ifmap index

    daddui R9, R0, 0        #temporary product
    daddui R12, R0, 0       #total sum
                            #R11 flag
    daddui R13, R0, 3       #x-offset

    daddui R15, R0, 0       #temp ifmap value
    daddui R16, R0, 0       #temp kernel value

column_loop:
    lb R15, ifmap(R8)       #load ifmap value
    lb R16, kernel(R7)      #load kernel value

    dmul R9, R15, R16       #multiplication kernel, ifmap
    dadd R12, R12, R9       #add to total sum

    daddui R7, R7, 1        #increment kernel index
    daddui R8, R8, 1        #increment ifmap index

    slt R11, R7, R10        #if kernel index > 8 restart kernel and next index in ofmap
    beqz R11, ofmap_loop
    slt R11, R8, R13        #if x-index > max xy offset change column (ifmap y-index)
    beqz R11, row_loop

    j column_loop

row_loop:
    dadd R4, R4, R1         #update x start position
    movz R8, R4, R0         #update ifmap index to new start
    dadd R13, R13, R1       #update max x-offset

    j column_loop

ofmap_loop:
    sb R12, ofmap(R6)
    daddui R7, R0, 0        #reset kernel index
    daddui R6, R6, 1        #increment ofmap index
    daddui R12, R0, 0       #reset total sum
    
    slt R11, R6, R10        #if ofmap index > 8 finish
    beqz R11, end
    
    #module division
    ddiv R18, R6, R17
    dmul R19, R18, R17
    dsub R11, R6, R19
    #ofmap_index % (mod) ofmap_row == 0 go to ifmap loop
    beqz R11, ifmap_loop

    daddui R3, R3, 1        #increment ifmap start

    j update_index

ifmap_loop:
    daddui R5, R5, 1        #increment y-start position
    dmul R3, R5, R1         #update ifmap start (y-index*dim ifmap row)
 
update_index:
    movz R8, R3, R0         #update ifmap index
    movz R4, R3, R0         #update ifmap index
    daddu R13, R8, R2       #update max xy offset

    j column_loop

end:
    halt

