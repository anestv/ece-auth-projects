%
% 100 000 data
% range: 0-5000. Most in range 0-500
% 500 klaseis me platos 1

N = size(raw, 1);
numClasses = 500;

edges = 0:1:numClasses;
classes = [];

for col = raw

	[c, e] = histcounts(col, edges);
	classes = [classes c'];

	%semilogx(e, [N 0])
end

classes = classes / N;

semilogx(e, [classes; zeros(1, 5)]);
title('Κατανομή πυκνότητας πιθανότητας του χρόνου αναμονής σε μs')

figure
semilogx(e, cumsum([classes; zeros(1, 5)]));
ylim([0 1])
title('Αθροιστική κατανομή πιθανότητας του χρόνου αναμονής σε μs')

raw = sort(raw);
third_quartile = raw(end * 3/4)
median_ = raw(end / 2)
max_ = raw(end)
stddev = std(raw)

