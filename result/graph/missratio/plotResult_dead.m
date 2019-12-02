%% File size
%x_label = [62.5	125	250	500	1000	2000	4000	8000]
%x_label = [200	400	600	800	1000	1200	1400	1600]
%x_label = [50	100	150	200	250	300	350	400]
%x_label =[ 0.5	1	2	4	8	16	32	64]
x_label =[ 1	2	4	8	16	32	64	128]



%% for DeadlinMissRatio
y_perfeect = [0.3256	0.2455	0.1828	0.1295	0.1166	0.0675	0.0382	0.0182];
y_unstable = [0.3619	0.2932	0.2075	0.1416	0.0819	0.04	0.0195	0.0083];


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
%xticklabels({'200','400','600','800','1000','1200','1400','1600'});
%xticklabels({'50','100','150','200','250','300','350','400'});
xticklabels({'1','2','4','8','16','32','64','128'});
set(gca, 'XTick', x_label);

legend({'Perfect Connection','Unstable Connection'},'NumColumns',1);

hold off;
grid on;