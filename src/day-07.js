const parseBag = new RegExp(/(\d+) (.+) bags?[,.]?/);
const parseBagList = (l) => l.split(',').map(x => x.match(parseBag)).map(([_, countStr, bag]) => ({count: +countStr, bag}));
const parseContainingBag = new RegExp(/(.+) bags contain (.*)/)
const parseLine = (l) => { const [_, bag, bagList] = l.match(parseContainingBag); return {bag, contents: bagList == 'no other bags.' ? [] : parseBagList(bagList)}; }
const bags = document.body.innerText.split('\n').filter(l => l.length > 0).map(parseLine);
const bagsContaining = (bag) => bags.filter(b => b.contents.filter(x => x.bag == bag).length > 0).map(x => x.bag).reduce((a, b) => [...new Set([...a, b, ...bagsContaining(b)])], []);
console.log('part 1:', bagsContaining("shiny gold").length)
const countBags = (bag) => bags.filter(x => x.bag == bag)[0].contents.map(y => y.count*countBags(y.bag)).reduce((x,y) => x+y, 0) + 1;
console.log('part 2:', countBags('shiny gold') - 1);
