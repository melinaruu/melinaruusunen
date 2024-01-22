const addToCart = (productId, productName) => {
  console.log('clicked')
  addProductToCart(productId);
  createNotification(`Added ${productName} to cart!`, 'notifications-container', true);
  // TODO 9.2
  // you can use addProductToCart(), available already from /public/js/utils.js
  // for showing a notification of the product's creation, /public/js/utils.js  includes createNotification() function
};

(async() => {
  const productsContainer = document.getElementById('products-container');
  const productTemplate = document.getElementById('product-template');
  const products = await getJSON('api/products');
  products.forEach(product => {
    let template = productTemplate.content.cloneNode(true);
    const id = product._id;
    template.querySelector('h3').innerText = product.name;
    template.querySelector('h3').setAttribute('id', `name-${id}`)
    template.querySelector('p.product-description').innerText = product.description;
    template.querySelector('p.product-description').id = `description-${id}`
    template.querySelector('p.product-price').innerText = product.price;
    template.querySelector('p.product-price').id = `price-${id}`;
    template.querySelector('button').id = `add-to-cart-${id}`;
    template.querySelector('button').addEventListener('click', ()=>addToCart(id, product.name));
    productsContainer.append(template);
  });
  //TODO 9.2 
  // - get the 'products-container' element from the /products.html
  // - get the 'product-template' element from the /products.html
  // - save the response from await getJSON(url) to get all the products. getJSON(url) is available to this script in products.html, as "js/utils.js" script has been added to products.html before this script file 
  // - then, loop throug the products in the response, and for each of the products:
  //    * clone the template
  //    * add product information to the template clone
  //    * remember to add an event listener for the button's 'click' event, and call addToCart() in the event listener's callback
  // - remember to add the products to the the page
})();