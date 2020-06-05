ETS = 1800;
ET0 = 150;
ETC = 150;
Et1 = 50;
Et2 = 0;

mus = 1/ETS;
lamda0 = 1/ET0;
lamdaC = 1/ETC;
gamma1 = 1/Et1;
gamma2 = 0;

P0 = ET0/(ETC + ET0)
q1 = Et1/(Et1+Et2)
q2 = 0
P1 = q1*(1-P0)
P2 = q2*(1-P0)

P01 = (q1*lamda0)/(lamda0+mus)
P02 = (q2*lamda0)/(lamda0+mus)
P10 = (lamdaC)/(lamdaC+gamma1+mus)
P12 = (gamma1)/(lamdaC+gamma1+mus)
P20 = (lamdaC)/(lamdaC+gamma2+mus)
P21 = (gamma2)/(lamdaC+gamma2+mus)

a21 = ((P01*P20)+P21)/(1-(P02*P20))
a01 = P01 + (P02)*a21
a12 = ((P02*P10)+P12)/(1-(P10*P01))
a02 = P02 + (P01)*a12

f1 = P10*(P01+(P02*(a21)))+ P12*a21
f2 = P20*(P02+(P01*(a12)))+ P21*a12

init = ((P0*a01)+P1+(P2*a21))

for n = 1:10
    str = "P[N1=" + n + "]=";
    pp = init*f1^(n-1);
    disp(str+pp)
end