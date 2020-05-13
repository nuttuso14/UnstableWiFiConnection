% case not
x_label =[0	30	60	125	225	360	450	600	900	1800	2400	3600]

%% for cdf
%N=100000
y_sim = [0	0.09348	0.1565	0.27832	0.4334	0.59272	0.67524	0.77624	0.89158	0.98728	0.99682	0.99988]
y_math = [0	0.070951	0.1347	0.257823	0.412981	0.570645	0.650345	0.749866	0.866875	0.960696	0.966141	0.96654]


i = i+1;
figure(i)

semilogx(x_label,y_sim,'-x', 'MarkerSize',10, 'LineWidth',1.3)
hold on
semilogx(x_label,y_math,'-s', 'MarkerSize',10, 'LineWidth',1.3)
hold on


xlabel('t (seconds)')
ylabel('CDF')


xticklabels({'0',	'30',	'60',	'125',	'225',	'360',	'450',	'600',	'900',	'1800',	'2400',	'3600'});
set(gca, 'XTick', x_label);
xlim([30, 3600]);
legend({'Simulation','Math Analysis'},'NumColumns',1);

hold off;
grid on;