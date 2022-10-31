.data
v1:     .double 1.34,4.638,2.62,1.93,1.243,8.969,7.455,6.933,8.336,1.573,5.497,8.403,9.004,0.987,0.5,0.156,9.678,3.941,6.573,0.797,5.676,7.796,9.825,5.62,9.645,2.596,4.017,1.72,8.425,8.322
        .double 4.295,1.123,0.978,8.861,9.491,3.775,5.294,7.055,4.927,3.885,6.708,1.514,0.726,3.022,3.41,6.825,4.729,4.524,4.162,9.369,0.603,2.206,1.27,3.661,6.442,7.395,1.259,0.031,0.265,7.57

v2:     .double 5.212,3.596,0.478,0.421,1.374,8.637,7.052,9.697,3.144,2.126,1.086,5.376,8.839,8.504,2.952,8.707,5.793,9.684,1.189,5.317,4.173,0.144,5.667,0.478,9.635,5.854,8.662,2.325,9.024,3.769
        .double 7.706,0.654,9.565,3.581,4.476,7.208,1.737,5.001,4.558,3.324,2.605,5.408,3.309,9.786,9.53,9.465,0.774,5.303,8.599,1.191,9.965,0.467,6.99,0.955,7.804,0.576,7.836,0.841,6.1,1.737

v3:     .double 4.023,4.473,2.319,8.557,1.105,3.898,0.306,8.183,2.757,6.729,6.43,8.786,0.183,4.291,6.177,8.633,0.426,7.996,7.963,3.898,6.998,6.01,5.552,6.764,0.85,2.441,0.594,9.183,8.537,9.725
        .double 9.065,8.588,2.177,2.005,2.44,4.231,5.16,2.613,0.901,0.113,3.011,2.663,1.439,0.783,9.805,6.336,0.91,0.154,8.573,2.978,0.567,8.269,1.185,0.916,0.167,8.808,6.372,7.285,9.787,6.346

v4:     .double 0.781,1.552,6.811,0.857,7.107,8.961,6.914,4.467,3.188,7.127,8.435,0.601,1.991,8.689,3.603,8.669,1.614,0.353,8.239,1.153,0.586,9.357,4.942,0.216,4.832,0.249,3.586,2.301,4.955,3.142
        .double 7.619,6.997,3.091,6.993,2.051,2.657,0.736,6.198,0.956,3.856,9.311,0.177,6.691,2.695,6.277,4.807,7.779,5.647,6.547,9.879,9.707,6.358,4.55,9.628,0.767,9.242,2.756,5.257,2.455,8.801
v5:     .space 480
v6:     .space 480
v7:     .space 480

.text
main:
    daddi R2, R0, 0         #vector offset
    daddi R1, R0, 60        #loop counter

loop:
	l.d F1, v1(R2)          #v1[i]
	l.d F2, v2(R2)          #v2[i]
    l.d F3, v3(R2)          #v3[i]

    add.d F5, F1, F2        #v1[i]+v2[i]

	l.d F4, v4(R2)          #v4[i]

    mul.d F8, F4, F1        #v4[i]*v1[i]

    daddi R1, R1, -1

    add.d F10, F2, F3       #v2[i]+v3[i]

	mul.d F6, F5, F3        #(v1[i]+v2[i])*v3[i]

	add.d F7, F6, F4        #v5[i] = ((v1[i]+v2[i])*v3[i])+v4[i]

	div.d F9, F7, F8        #v6 = v5[i]/(v4[i]*v1[i])

    s.d F7, v5(R2)
	s.d F9, v6(R2)

	mul.d F11, F9, F10      #v7[i] = v6[i]*(v2[i]+v3[i])

	daddi R2, R2, 8

    bnez R1, loop
	s.d F11, v7(R2)

	halt