_8=/(\d+) (.+) bags?[,.]?/;syzz=/(.+) bags contain (.*)/;
z=new RegExp(_8);_2=(l)=>l.split(',').map(xp=>xp.match(z)
).map                        (([fullLines,countString,bag
])=>(                        {count:+countString,bag:bag}
));zzzzzzzzzz=new RegExp     (syzz);_4=(l)=>{const[__,bag       
,bxs]=l.match(zzzzzzzzzz     );return({bag,content:bxs[0]
=='n'?[]:_2(bxs)});};u=     document.body.innerText.split
('\n').filter(lsx=>lsx     .length>0).map(_4);_6=(bag)=>u
.filter(ooooo=>ooooo.     content.filter(xp=>xp.bag==bag)
.length>0).map(x=>x     .bag).reduce((a,b)=>[...new Set([
...a,b,..._6(b)])]     ,[]);console.log('part 1 solution'
,_6("shiny gold")     .length);_7=(bag)=>u.filter(xy=>xy.
bag==bag)[0].content.map(y=>y.count*_7(y.bag)).reduce((x,
y)=>x+y,0)+1;console.log('part 2 is',_7('shiny gold')-1);
