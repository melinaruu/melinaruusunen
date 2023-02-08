// updates given HTML

const contactTemplate = document.getElementById("user-card-template");
const contacts = document.getElementById("contacts");

document.addEventListener('userDataReady', function (event) {
    const data = event.detail.jsonText
    const parsed = JSON.parse(data)

    for (var i=0 ; i<parsed.length ; i++) {
        let template = contactTemplate.content.cloneNode(true);

        const firstname = parsed[i].firstName;
        const lastname = parsed[i].lastName;
        const email = parsed[i].email;
        const phonenro = parsed[i].phoneNumber;
        const streetaddress = parsed[i].address.streetAddress;
        const zipcode = parsed[i].address.zipCode;
        const city = parsed[i].address.city;
        const country = parsed[i].address.country;
        const url = parsed[i].homepage;
        const avatar = parsed[i].avatar;
        const username = parsed[i].username;

        template.querySelector("h1").innerText = String(firstname + " " + lastname)
        template.querySelector("p.email").innerText = email
        template.querySelector("p.phone").querySelector("span").innerText = phonenro
        template.querySelector("div.address").children[0].innerText = streetaddress
        template.querySelector("div.address").children[1].innerText = String(zipcode + " " + city)
        template.querySelector("div.address").children[2].innerText = country
        template.querySelector("p.homepage").firstChild.innerText = url
        template.querySelector("p.homepage").firstChild.setAttribute("href", url)
        template.querySelector("img").src = avatar
        template.querySelector("img").alt = String(firstname + " " + lastname)

        contacts.append(template)
    }
});

fetchUserData();