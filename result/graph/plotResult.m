x_label = [50 100 200 400 800 1600 3200 6400]

%% for Offratio
y_perfeect = [0.971562	0.944015	0.890341	0.804883	0.677841	0.506733	0.345581	0.202195];
y_unstable = [0.648156	0.641398	0.594016	0.533309	0.454198	0.33601	0.230912	0.134402];

i = i+1;
figure(i)

semilogx(x_label,y_perfeect,'-x', 'MarkerSize',10, 'LineWidth',1.3)
hold on
semilogx(x_label,y_unstable,'-s', 'MarkerSize',10, 'LineWidth',1.3)
hold on

%xlabel('E[\tau_g]')
%xlabel('E[\tau_b]')
%xlabel('E[t_1]')
xlabel('E[t_0]')
ylabel('WiFi Offloading Ratio')

xticklabels({'50','100','200','400','800','1600','3200','6400'});
set(gca, 'XTick', x_label);

legend({'Perfect Connection','Unstable Connection'},'NumColumns',1);

hold off;
grid on;