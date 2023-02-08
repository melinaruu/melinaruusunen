// counter for what numbers come out of the dice and updates the HTML that was given

let rollBtn = document.getElementById('roll-button').addEventListener('click', event => {
    rollDice();
});

document.addEventListener('rollDice', function (e) {
    const num = e.detail.value

    const rollTemplate = document.getElementById(String("template"+num))
    let template = rollTemplate.content.cloneNode(true)

    const correctDie = template.querySelector("span").innerHTML
    
    var allSpans = document.getElementsByTagName("span")

    for(var i=0;i<allSpans.length;i++){
        if (allSpans[i].innerHTML === correctDie) {
            var parent = allSpans[i].parentNode
            break
        }
    }

    if (parent.querySelector("p").innerText === "-") {
        parent.querySelector("p").innerText = String(1)
    } else {
        const amount = parseInt(parent.querySelector("p").innerText)+1
        parent.querySelector("p").innerText = String(amount)
    }
    
    if (document.getElementById("totals").querySelector("span").innerText === "NaN") {
        document.getElementById("totals").querySelector("span").innerText = String(0)
    } else {
        const total = parseInt(document.getElementById("totals").querySelector("span").innerText)+1
        document.getElementById("totals").querySelector("span").innerText = String(total)
    }
    
    document.getElementById("dice").querySelector("span").innerHTML="";
    document.getElementById("dice").querySelector("span").append(correctDie)
});