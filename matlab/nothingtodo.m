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
P21 = (gamma2)/(lamdaC+gamma2)

%% For state 1

pi = [P0 P1 P2]

P = [0 P01 P02;P10 0  P12;P20 P21 0]

alpha = P^100
newpi = pi*alpha

P01_2 = (q1*lamda0)/(lamda0+mus);
P02_2 = (q2*lamda0)/(lamda0+mus);
P10_2 = (lamdaC)/(lamdaC+gamma1+mus);
P12_2 = (gamma1)/(lamdaC+gamma1+mus);
P20_2 = (lamdaC)/(lamdaC+gamma2+mus);
P21_2 = (gamma2)/(lamdaC+gamma2+mus);

inf_2002 = P20*P02*P20*P02*P20*P02*P20*P02;
inf_0220 = P02*P20*P02*P20*P02*P20*P02*P20;


test_1 = P0*P01_2 + P0*P21_2*inf_2002 + P0*P01_2*inf_0220  + P1 + P2*P21_2 + P2*P21_2*inf_2002 + P2*P01_2*inf_0220
