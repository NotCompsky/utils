// to merge two reddit threads

// for localcache version:  JSON.stringify(Array.from(document.getElementsByClassName("bylink")).map(x => x.href.split("/")[12]).filter(x => !x.startsWith("#")))

let old_ids=["kyi4a0z","kyilmze","kyitd0j","kyiwqts","kyixumq","kyj57iu","kyjydsj","kyj2can","kyiau33","kyifqhx","kyiim0o","kyikb28","kyj7gi8","kyk1g1o","kyim9f8","kyjiqcf","kyim54c","kyimsm5","kyirnj4","kyk0125","kyj6o5c","kyipukq","kyi5617","kyijmn8","kyink89","kyin98z","kyjnlqc","kyi7jd9","kyibgag","kyijjn6","kyir8xf","kyj0n76","kyja4i0","kyj4o30","kyixnc4","kyjyh0b","kyk0u5e","kyieddq","kyimslp","kyipe32","kyiug17","kyj064t","kyiv634","kyiwn23","kyjz8i3","kyj0ak7","kyj0yqw","kyja5ac","kyj6za9","kyjazlm","kyjsrjv","kyj9trz","kyjz3as","kyk0jv6","kyj9iyj","kyjyv37","kyj93og","kyi3jve","kyi4hld","kyi9y8y","kyiale3","kyim9i0","kyibijl","kyieux3","kyjrw6t","kyidvht","kyikg4l","kyimy6x","kyjald0","kyjjt7f","kyjl9hm","kyjlqn4","kyjmjk7","kyi5frv","kyi5sdj","kyjam82","kyi65m4","kyiyd18","kyi72we","kyjjxat","kyijkzw","kyiftti","kyip9uo","kyia7iz","kyicgfj","kyid6ct","kyihs6u","kyiatfu","kyib4v2","kyifimc","kyj1k7p","kyjb21q","kyjpipf","kyjucjf","kyilb9t","kyimp9c","kyin3ju","kyirgfj","kyitvvh","kyivc4v","kyixfa6","kyiyd4d","kyj0pow","kyj8eqf","kyisbca","kyiw287","kyj9t61","kyj2nni","kyiugm3","kyiwcvh","kyixsy6","kyiygk8","kyj0jxh","kyj0225","kyj19py","kyivsqy","kyiw8vr","kyiwroc","kyj1fxl","kyi91cq","kyjophb","kyj8j8m","kyiinvt","kyis23r","kyiscxo","kyit3gm","kyjfq4r","kyjjd57","kyjj9df","kyk1xm8","kyia4ei","kyiboxv","kyii4p9","kyik2pg","kyimf3c","kyizpug","kyihiku","kyi2btj","kyi2d0i","kyimfr4","kyj5v01","kyienw9","kyjzfmc","kyiquoy"];


Array.from(document.getElementsByClassName("bylink")).map(x => x.href.split("/")[8]).filter(x => ((!x.startsWith("#"))&&(!old_ids.includes(x)))).map(x => document.getElementById("thing_t1_"+x));

then step through each element

Array.from(document.getElementsByClassName("bylink")).map(x => x.href.split("/")[8]).filter(x => ((!x.startsWith("#"))&&(!old_ids.includes(x)))).map(x => document.getElementById("thing_t1_"+x))[i++].scrollIntoView();
