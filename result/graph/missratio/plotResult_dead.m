%% File size
%x_label = [62.5	125	250	500	1000	2000	4000	8000]
%x_label = [10	20	40	80	160	320	640	1280]
%x_label = [50	100	150	200	250	300	350	400]
%x_label =[ 0.5	1	2	4	8	16	32	64]
x_label =[ 1	2	4	8	16	32	64	128]



%% for DeadlinMissRatio
y_perfeect = [0.7636	0.6065	0.4201	0.2429	0.1088	0.0444	0.0174	0.0051];
y_unstable = [0.841	0.7308	0.5644	0.3911	0.204	0.0947	0.0392	0.012];


i = i+1;
figure(i)

semilogx(x_label,y_perfeect,'-x', 'MarkerSize',10, 'LineWidth',1.3)
hold on
semilogx(x_label,y_unstable,'-s', 'MarkerSize',10, 'LineWidth',1.3)
hold on

%xlabel('E[\tau_g]')
%xlabel('E[\tau_b]')
%xlabel('E[t_1]')
%xlabel('File Size : MB')
%xlabel('E[t_0]')
%xlabel('b_1(Mbps)')
xlabel('b_2(Mbps)')
ylabel('DeadLine Miss Ratio')

%xticklabels({'62.5','125','250','500','1000','2000','4000','8000'});
%xticklabels({'10','20','40','80','160','320','640','1280'});
%xticklabels({'50','100','150','200','250','300','350','400'});
xticklabels({'1','2','4','8','16','32','64','128'});
set(gca, 'XTick', x_label);

legend({'Perfect Connection','Unstable Connection'},'NumColumns',1);

hold off;
grid on;