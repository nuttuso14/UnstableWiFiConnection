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

%% For state 1

pi = [P0 P1 P2]

P = [0 P01 P02;P10 0  P12;P20 P21 0]

alpha = P^10
newpi = pi*alpha

P01_2 = (q1*lamda0)/(lamda0+mus);
P02_2 = (q2*lamda0)/(lamda0+mus);
P10_2 = (lamdaC)/(lamdaC+gamma1+mus);
P12_2 = (gamma1)/(lamdaC+gamma1+mus);
P20_2 = (lamdaC)/(lamdaC+gamma2+mus);
P21_2 = (gamma2)/(lamdaC+gamma2+mus);

alpha21 = ((alpha(1,2)*alpha(3,1))+alpha(3,2))/(1-(alpha(1,3)*alpha(3,1)))
alpha01 = P01_2 + P02_2*alpha21
alpha12 = ((alpha(1,3)*alpha(2,1))+alpha(2,3))/( 1-(alpha(2,1)*alpha(1,2)))
alpha02 = P02_2 + P01_2*alpha12




%f1 = P10_2*(alpha01) + P12_2*alpha21;
f1 = P10_2*(alpha(1,2)+(alpha(1,3)*alpha(3,2))) +P12_2*(alpha(3,2)+(alpha(3,1)*alpha(1,2))); 
%f1 = P10_2*(alpha(1,2)) +P12_2*(alpha(3,2)); 
%f1 = P10_2*(P01_2+newpi(2)) +P12_2*(P21_2+newpi(2)); 
%f1 = P10_2*(P01_2+(alpha(1,3)*alpha(3,2))) +P12_2*(P21_2+(alpha(3,1)*alpha(1,2))); 
test_0 = q2*(1-P21_2) ; 
%test_1 = P0*alpha01 + P1 + P2*alpha21
%test_1 = P0*(newpi(2)) + P1 + P2*(newpi(2))
test_1 = P0*alpha(1,2)+P1+P2*alpha(3,2) ;


test_2 = test_1*f1;
test_3 = test_1*(f1^2);
test_4 = test_1*(f1^3);
test_5 = test_1*(f1^4);
test_6 = test_1*(f1^5);


%f2 = P20_2*(alpha02) + P21*alpha12;
%f2 = P20_2*(alpha(3)+ (alpha(4)*alpha(8))) + P21_2*(alpha(8)+ (alpha(2)*alpha(7)));
%f2 = P20_2*(alpha(3,1)+ (alpha(1,2)*alpha(2,3))) + P21_2*(alpha(2,3)+ (alpha(2,1)*alpha(1,3)));
f2 = P20_2*(alpha(1,3)+ (alpha(1,2)*alpha(2,3))) + P21_2*(alpha(2,3)+ (alpha(2,1)*alpha(1,3)));
test2_0 = q1*(1-P12_2) ; 
%test2_1 = P0*alpha02 + P1*alpha12 + P2
%test2_1 = P0*alpha(7)+P1*alpha(8) + P2;
test2_1 = P0*newpi(3)+P1*newpi(3) + P2;
test2_2 = test2_1*f2;
test2_3 = test2_1*(f2^2);
test2_4 = test2_1*(f2^3);
test2_5 = test2_1*(f2^4);
test2_6 = test2_1*(f2^5);

t1 = [test_0 test_1 test_2 test_3 test_4 test_5 test_6]
t2 = [test2_0 test2_1 test2_2 test2_3 test2_4 test2_5 test2_6]
