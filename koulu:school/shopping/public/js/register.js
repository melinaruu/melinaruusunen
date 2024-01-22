/**
 * TODO: 8.4 Register new user
 *       - Handle registration form submission
 *       - Prevent registration when password and passwordConfirmation do not match
 *       - Use createNotification() function from utils.js to show user messages of
 *       - error conditions and successful registration
 *       - Reset the form back to empty after successful registration
 *       - Use postOrPutJSON() function from utils.js to send your data back to server
 */ 
const form = document.getElementById('register-form');

const handleSubmit = async (event) => {
    event.preventDefault()
    const formdata = new FormData(form);

    const name = formdata.get('name');
    const email = formdata.get('email');
    const password = formdata.get('password');
    const passwordConfirmation = formdata.get('passwordConfirmation');
    const data = {
        name: name,
        email: email,
        password: password,
    }
    if(password !== passwordConfirmation){
        createNotification("Passwords dont match", 'register-form', false);
    }
    try{
        const response = await postOrPutJSON('/api/register', 'POST', data);
        createNotification('created', 'register-form', true)
    }catch(error){
        createNotification('Something went wrong :(', 'register-form', false)
    }
    
    form.reset();
}

document.getElementById('btnRegister').addEventListener('click', handleSubmit)









