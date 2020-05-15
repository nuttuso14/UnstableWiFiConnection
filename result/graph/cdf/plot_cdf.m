% case not
x_label =[30	60	120	180	240	480	600	900	1800	2400	3600]

%% for cdf
%N=100000
y_sim = [0.0933	0.15632	0.27544	0.36902	0.45942	0.69584	0.76928	0.89114	0.98806	0.99686	0.99988]
y_math = [0.0898757	0.171003	0.28861	0.352187	0.466232	0.727385	0.793114	0.886339	0.974135	0.972505	0.973775]


i = i+1;
figure(i)

semilogx(x_label,y_sim,'-x', 'MarkerSize',10, 'LineWidth',1.3)
hold on
semilogx(x_label,y_math,'-s', 'MarkerSize',10, 'LineWidth',1.3)
hold on


xlabel('t (seconds)')
ylabel('CDF')


xticklabels({'30'	,'60'	,'120'	,'180'	,'240'	,'480'	,'600'	,'900'	,'1800'	,'2400'	,'3600'});
set(gca, 'XTick', x_label);
xlim([30, 3600]);
legend({'Simulation','Math Analysis'},'NumColumns',1);

hold off;
grid on;