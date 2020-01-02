%% File size
% case equal
%x_label =[0 1 2 3 4 5 6 7 8]
% case not
x_label =[0	1	2	3	4	5	6	7	8	9	10	11	12]

%% for DeadlinMissRatio
%N=100000
y_sim = [0.069158	0.581504	0.218574	0.081517	0.030695	0.011698	0.004247	0.001647	0.000655	0.000184	0.000095	0.000013	0.000013]
y_math = [0.070081	0.542705	0.22598	0.094097	0.039182	0.016315	0.006794	0.002829	0.001178	0.00049	0.000204	0.000085	0.000035]


i = i+1;
figure(i)

plot(x_label,y_sim,'-x', 'MarkerSize',10, 'LineWidth',1.3)
hold on
plot(x_label,y_math,'-s', 'MarkerSize',10, 'LineWidth',1.3)
hold on

%xlabel('E[\tau_g]')
%xlabel('E[\tau_b]')
%xlabel('E[t_1]')
%xlabel('File Size : MB')
%xlabel('E[t_0]')
%xlabel('b_1(Mbps)')
xlabel('n')
ylabel('P[N=n]')

%xticklabels({'62.5','125','250','500','1000','2000','4000','8000'});
%xticklabels({'10','20','40','80','160','320','640','1280'});
%xticklabels({'50','100','150','200','250','300','350','400'});

%xticklabels({'0',	'1',	'2',	'3',	'4','5',	'6',	'7',	'8',	'9',	'10',	'11',	'12',	'13',	'14',	'15',	'16',	'17'	,'18'	,'20'	,'21'	,'22',	'23',	'24',	'26'});
%set(gca, 'XTick', x_label);

legend({'Simulation','Math Model'},'NumColumns',1);

hold off;
grid on;