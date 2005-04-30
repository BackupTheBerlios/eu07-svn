
new nmcdynamic $name
	sel $name

	.settype real

	new ncamerahandle ext1
	ext1.setoffset 0  0 6
	ext1.setr 20

	new ncamerahandle ext2
	ext2.setoffset 5  0 1.8


	new nosgmodel visual
	visual.setmodel dynamic/pkp/ty2/ty2beczka.ive ""



	.setvisual visual

	sel audio
	sel ..

	.setnumaxles 4
	.setstdparams 3.886 14 1

	# PARAMETRY FIZYKI
	# podstawowe parametry: kategoria, masa [kg], Vmax [km/h], Pmax silnika [kW], Fmax silnika [kN] 
	.basicparam train 83500 125 500 175
	# wymiary: dlugosc, wysokosc, szerokosc, Cx
	.sizeparam 8.6 3.9 2.903 0.3 

	# urz. zd-sprz: przod/tyl, zderzak/sprzeg, typ, k [kN/m], d [m], Fmax [kN], tlum.dyn. [kN*s/m], tlum.stat. []
	.interface 0 buffer dual 9200 0.15 820 100 0.58 
	.interface 0 coupler screw 4800 0.15 780 100 0.58
	.interface 1 buffer dual 9200 0.15 820 100 0.58 
	.interface 1 coupler screw 4800 0.15 780 100 0.58

	# nastawniki: masterctrl, scndctrl, mainbrake, helperbrake
	.ctrlparam 42 6 6 10 

	# kola, dane ogolne: rozstaw [m], uklad, odleglosc osi[m], odleglosc czopow [m], min R [m], typ lozyska, sila docisku klocka
	.wheelparam 1.435 "Bo'Bo'" 1 3.886 80 "Roll" 70000

	# kola: srednica, moment bezwladnosci, przekladnia, stopien obciazenia, ilosc klockow na os, kontrola predkosci 
#	.axleparam 1 -0.5 0.363 2000.0 [expr 79.0/18.0] 0.25 4 1
#	.axleparam 2 0.5 1.646 2000.0 [expr 79.0/18.0] 0.25 4 0
	.axleparam 1 -2.5613 0.363 2000.0 0 0.25 4 1
	.axleparam 2 -0.76 1.646 2000.0 0 0.25 4 0
	.axleparam 3 1.58 1.646 2000.0 0 0.25 4 0
	.axleparam 4 3.29 1.646 2000.0 0 0.25 4 0

