
	.settype real


	new nosgmodel visual
	visual.setmodel dynamic/pkp/eu07/EU07-303E-m.ive "dynamic/pkp/eu07/399"

	.setvisual visual

	.setnumaxles 4
	# kola: odleglosc, srednica, moment bezwladnosci, przekladnia, stopien obciazenia, ilosc klockow na os, kontrola predkosci 
	.axleparam 1 [expr (-3.05-8.55)*0.5] 1.250 2000.0 [expr 79.0/18.0] 0.25 4 1
	.axleparam 2 [expr ( 3.05-8.55)*0.5] 1.250 2000.0 [expr 79.0/18.0] 0.25 4 0
	.axleparam 3 [expr (-3.05+8.55)*0.5] 1.250 2000.0 [expr 79.0/18.0] 0.25 4 0
	.axleparam 4 [expr ( 3.05+8.55)*0.5] 1.250 2000.0 [expr 79.0/18.0] 0.25 4 0


	.setstdparams 8.55 16.260 2
	.initpantograph 1 0 4.2486  3.3186  0.8
	.initpantograph 2 0 4.2486 -3.3186  0.8

