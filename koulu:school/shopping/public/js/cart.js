const addToCart = productId => {
  addProductToCart(productId);
  updateProductAmount(productId);
  // TODO 9.2
  // use addProductToCart(), available already from /public/js/utils.js
  // call updateProductAmount(productId) from this file
};

const decreaseCount = productId => {
  const count = decreaseProductCount(productId);
  updateProductAmount(productId)
  if(count === 0){
    removeElement('cart-container', `item-${productId}`)
  }
  // TODO 9.2
  // Decrease the amount of products in the cart, /public/js/utils.js provides decreaseProductCount()
  // Remove product from cart if amount is 0,  /public/js/utils.js provides removeElement = (containerId, elementId

};

const updateProductAmount = productId => {
  const count = getProductCountFromCart(productId);
  document.querySelector(`#amount-${productId}`).innerText = count+'x'
  // TODO 9.2
  // - read the amount of products in the cart, /public/js/utils.js provides getProductCountFromCart(productId)
  // - change the amount of products shown in the right element's innerText

};

const placeOrder = async() => {
  const products = getAllProductsFromCart();
  createNotification('Successfully created an order!', 'notifications-container', true)
  products.forEach(product => {
    removeElement('cart-container', `item-${product.id}`)
  })
  clearCart();
  // TODO 9.2
  // Get all products from the cart, /public/js/utils.js provides getAllProductsFromCart()
  // show the user a notification: /public/js/utils.js provides createNotification = (message, containerId, isSuccess = true)
  // for each of the products in the cart remove them, /public/js/utils.js provides removeElement(containerId, elementId)
};

(async() => {
  document.querySelector('#place-order-button').addEventListener('click', placeOrder)
  const cartContainer = document.getElementById('cart-container');
  const products = await getJSON('api/products');
  const cartProducts = getAllProductsFromCart();
  const productTemplate = document.getElementById('cart-item-template')
  cartProducts.forEach(product => {
    let template = productTemplate.content.cloneNode(true);
    const id = product.id;
    const originalProduct = products.find(e => e._id === product.id)
    template.querySelector('div.item-row').id = `item-${id}`;
    template.querySelector('h3').innerText = originalProduct.name;
    template.querySelector('h3').id = `name-${id}`
    template.querySelector('p.product-price').innerText = originalProduct.price;
    template.querySelector('p.product-price').id = `price-${id}`;
    template.querySelector('p.product-amount').innerText = product.amount +'x';
    template.querySelector('p.product-amount').id = `amount-${id}`;
    const buttons = template.querySelectorAll('button.cart-minus-plus-button');
    console.log(buttons)
    buttons[0].addEventListener('click', ()=>addToCart(id));
    buttons[0].id = `plus-${id}`
    buttons[1].addEventListener('click', ()=>decreaseCount(id));
    buttons[1].id = `minus-${id}`;

    cartContainer.append(template);
  });
  // TODO 9.2
  // - get the 'cart-container' element
  // - use getJSON(url) to get the available products
  // - get all products from cart
  // - get the 'cart-item-template' template
  // - for each item in the cart
  //    * copy the item information to the template
  //    * hint: add the product's ID to the created element's as its ID to 
  //        enable editing ith 
  //    * remember to add event listeners for cart-minus-plus-button
  //        cart-minus-plus-button elements. querySelectorAll() can be used 
  //        to select all elements with each of those classes, then its 
  //        just up to finding the right index.  querySelectorAll() can be 
  //        used on the clone of "product in the cart" template to get its two
  //        elements with the "cart-minus-plus-button" class. Of the resulting
  //        element array, one item could be given the ID of 
  //        `plus-${product_id`, and other `minus-${product_id}`. At the same
  //        time we can attach the event listeners to these elements. Something 
  //        like the following will likely work:
  //          clone.querySelector('button').id = `add-to-cart-${prodouctId}`;
  //          clone.querySelector('button').addEventListener('click', () => addToCart(productId, productName));
  //
  // - in the end remember to append the modified cart item to the cart 

})();