ETS = 1800;
ET0 = 150;
ETC = 150;
Et1 = 50;
Et2 = 100;

mus = 1/ETS;
lamda0 = 1/ET0;
lamdaC = 1/ETC;
gamma1 = 1/Et1;
gamma2 = 1/Et2;

P0 = ET0/(ETC + ET0);
q1 = Et1/(Et1+Et2);
q2 = Et2/(Et1+Et2);
P1 = q1*(1-P0);
P2 = q2*(1-P0);
P01 = (q1*lamda0)/(lamda0);
P02 = (q2*lamda0)/(lamda0);

P10 = (lamdaC)/(lamdaC+gamma1);
P12 = (gamma1)/(lamdaC+gamma1);

P20 = (lamdaC)/(lamdaC+gamma2);
P21 = (gamma2)/(lamdaC+gamma2);


P = [0 q1 q2;P10 0  P12;P20 P21 0]
alpha = P^50

alpha(4)
alpha(6)

P10_2 = (lamdaC)/(lamdaC+gamma1+mus);
P12_2 = (gamma1)/(lamdaC+gamma1+mus);


test_0 = P0*alpha(7)+P2*alpha(3) 
test_1 = P0*alpha(4)+P1+P2*alpha(6) 

%f1 = P10*alpha(4) + P12*alpha(6) 
%f1 = P10*(alpha(4)+(alpha(7)*alpha(6)))+ + P12*(alpha(6)+(alpha(3)*alpha(4))) 
f1 = P10_2*(alpha(4)+(alpha(7)*alpha(6)))+ + P12_2*(alpha(6)+(alpha(3)*alpha(4))) 
test_2 = test_1*f1
test_3 = test_1*(f1^2)
test_4 = test_1*(f1^3)
test_5 = test_1*(f1^4)
