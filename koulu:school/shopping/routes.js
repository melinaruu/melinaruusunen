const responseUtils = require('./utils/responseUtils');
const { acceptsJson, isJson, parseBodyJson, getCredentials } = require('./utils/requestUtils');
const { renderPublic } = require('./utils/render');
const { emailInUse, getAllUsers, saveNewUser, validateUser, updateUserRole, getUser, getUserById, deleteUserById} = require('./utils/users');
const products = require('./products.json').map(product => ({...product}));
/**
 * Known API routes and their allowed methods
 *
 * Used to check allowed methods and also to send correct header value
 * in response to an OPTIONS request by sendOptions() (Access-Control-Allow-Methods)
 */
const allowedMethods = {
  '/api/register': ['POST'],
  '/api/users': ['GET'],
  '/api/products': ['GET']
};


const authenticateAdmin = (response, credentials) => {
  if(credentials === null){
    responseUtils.basicAuthChallenge(response);
    responseUtils.unauthorized(response);
  }else if(credentials.length !==2){
    responseUtils.basicAuthChallenge(response);
    responseUtils.unauthorized(response);
    
  }else if(getUser(credentials[0], credentials[1]) === undefined){
    responseUtils.basicAuthChallenge(response);
    responseUtils.unauthorized(response);
  }
  else if(getUser(credentials[0], credentials[1]).role !== 'admin' && getUser(credentials[0], credentials[1]).role !== 'customer'){
    responseUtils.forbidden(response)
  }else{
    return true
  }
}
/**
 * Send response to client options request.
 *
 * @param {string} filePath pathname of the request URL
 * @param {http.ServerResponse} response
 */
const sendOptions = (filePath, response) => {
  if (filePath in allowedMethods) {
    response.writeHead(204, {
      'Access-Control-Allow-Methods': allowedMethods[filePath].join(','),
      'Access-Control-Allow-Headers': 'Content-Type,Accept',
      'Access-Control-Max-Age': '86400',
      'Access-Control-Expose-Headers': 'Content-Type,Accept'
    });
    return response.end();
  }

  return responseUtils.notFound(response);
};

/**
 * Does the url have an ID component as its last part? (e.g. /api/users/dsf7844e)
 *
 * @param {string} url filePath
 * @param {string} prefix
 * @returns {boolean}
 */
const matchIdRoute = (url, prefix) => {
  const idPattern = '[0-9a-z]{8,24}';
  const regex = new RegExp(`^(/api)?/${prefix}/${idPattern}$`);
  return regex.test(url);
};

/**
 * Does the URL match /api/users/{id}
 *
 * @param {string} url filePath
 * @returns {boolean}
 */
const matchUserId = url => {
  return matchIdRoute(url, 'users');
};

const handleRequest = async(request, response) => {
  const { url, method, headers } = request;
  const filePath = new URL(url, `http://${headers.host}`).pathname;

  // serve static files from public/ and return immediately
  if (method.toUpperCase() === 'GET' && !filePath.startsWith('/api')) {
    const fileName = filePath === '/' || filePath === '' ? 'index.html' : filePath;
    return renderPublic(fileName, response);
  }

  if (matchUserId(filePath)) {
    const id = filePath.substring(11);
    if(method.toUpperCase() === 'GET'){
      const credentials = getCredentials(request);
    if(credentials === null){
      responseUtils.basicAuthChallenge(response);
      responseUtils.unauthorized(response);
    }else if(credentials.length !==2){
      responseUtils.basicAuthChallenge(response);
      responseUtils.unauthorized(response);
      
    }else if(getUser(credentials[0], credentials[1]) === undefined){
      responseUtils.basicAuthChallenge(response);
      responseUtils.unauthorized(response);
    }
    else if(getUser(credentials[0], credentials[1]).role !== 'admin'){
      responseUtils.forbidden(response)
    }else{
      const result = getUserById(id);
      if(result !== undefined ){
        responseUtils.sendJson(response, result, 200)
      }else{
        responseUtils.notFound(response);
      }

    }
    }
    if(method.toUpperCase() === 'PUT'){
        const credentials = getCredentials(request);
      if(credentials === null){
        responseUtils.basicAuthChallenge(response);
        responseUtils.unauthorized(response);
      }else if(credentials.length !==2){
        responseUtils.basicAuthChallenge(response);
        responseUtils.unauthorized(response);
        
      }else if(getUser(credentials[0], credentials[1]) === undefined){
        responseUtils.basicAuthChallenge(response);
        responseUtils.unauthorized(response);
      }
      else if(getUser(credentials[0], credentials[1]).role !== 'admin'){
        responseUtils.forbidden(response)
      }else{
        const user = await parseBodyJson(request);
        if(user.role){
          try{
            const result = updateUserRole(id, user.role);
            if(result === undefined){
              responseUtils.notFound(response);
            }else{
              responseUtils.sendJson(response, result, 200)
            }    
          }catch(e){
            responseUtils.badRequest(response,'invalid')
          }
          
        }else{
          responseUtils.badRequest(response,'invalid')
        }
  
      }
    }
    if(method.toUpperCase() === 'DELETE'){
      const credentials = getCredentials(request);
    if(credentials === null){
      responseUtils.basicAuthChallenge(response);
      responseUtils.unauthorized(response);
    }else if(credentials.length !==2){
      responseUtils.basicAuthChallenge(response);
      responseUtils.unauthorized(response);
      
    }else if(getUser(credentials[0], credentials[1]) === undefined){
      responseUtils.basicAuthChallenge(response);
      responseUtils.unauthorized(response);
    }
    else if(getUser(credentials[0], credentials[1]).role !== 'admin'){
      responseUtils.forbidden(response)
    }else{
      const user = getUserById(id);
      if(user === undefined){
        responseUtils.notFound(response);
      }
      const result = deleteUserById(id);
      responseUtils.sendJson(response, result, 200)

    }
    }
    // TODO: 8.6 Implement view, update and delete a single user by ID (GET, PUT, DELETE)
    // You can use parseBodyJson(request) from utils/requestUtils.js to parse request body
    // If the HTTP method of a request is OPTIONS you can use sendOptions(filePath, response) function from this module
    // If there is no currently logged in user, you can use basicAuthChallenge(response) from /utils/responseUtils.js to ask for credentials
    //  If the current user's role is not admin you can use forbidden(response) from /utils/responseUtils.js to send a reply
    // Useful methods here include:
    // - getUserById(userId) from /utils/users.js
    // - notFound(response) from  /utils/responseUtils.js 
    // - sendJson(response,  payload)  from  /utils/responseUtils.js can be used to send the requested data in JSON format
  }

  // Default to 404 Not Found if unknown url
  if (!(filePath in allowedMethods)) return responseUtils.notFound(response);

  // See: http://restcookbook.com/HTTP%20Methods/options/
  if (method.toUpperCase() === 'OPTIONS') return sendOptions(filePath, response);

  // Check for allowable methods
  if (!allowedMethods[filePath].includes(method.toUpperCase())) {
    return responseUtils.methodNotAllowed(response);
  }

  // Require a correct accept header (require 'application/json' or '*/*')
  if (!acceptsJson(request)) {
    return responseUtils.contentTypeNotAcceptable(response);
  }

  // GET all users
  if (filePath === '/api/users' && method.toUpperCase() === 'GET') {
    
    const credentials = getCredentials(request);
    if(credentials === null){
      responseUtils.basicAuthChallenge(response);
      responseUtils.unauthorized(response);
    }else if(credentials.length !==2){
      responseUtils.basicAuthChallenge(response);
      responseUtils.unauthorized(response);
      
    }else if(getUser(credentials[0], credentials[1]) === undefined){
      responseUtils.basicAuthChallenge(response);
      responseUtils.unauthorized(response);
    }
    else if(getUser(credentials[0], credentials[1]).role !== 'admin'){
      responseUtils.forbidden(response)
    }
    else{
      return responseUtils.sendJson(response, getAllUsers());
    }
    // TODO: 8.5 Add authentication (only allowed to users with role "admin")
    
  }
  if(filePath === '/api/products' && method.toUpperCase() === 'GET'){
    const credentials = getCredentials(request);
    if(authenticateAdmin(response, credentials)){
      responseUtils.sendJson(response, products, 200);
    }
  }

  // register new user
  if (filePath === '/api/register' && method.toUpperCase() === 'POST') {
    // Fail if not a JSON request, don't allow non-JSON Content-Type
    if (!isJson(request)) {
      return responseUtils.badRequest(response, 'Invalid Content-Type. Expected application/json');
    }
    const user = await parseBodyJson(request);
    const errors = validateUser(user);
    if(errors.length !== 0){
      responseUtils.badRequest(response, '400 Bad Request')
    }else if(emailInUse(user.email)){
      responseUtils.badRequest(response, '400 Bad Request')
    }else{
      user.role = 'customer'
      const result = saveNewUser(user)
      responseUtils.createdResource(response, result)
    }
    

    
    
    // TODO: 8.4 Implement registration
    // You can use parseBodyJson(request) method from utils/requestUtils.js to parse request body.
    // Useful methods here include:
    // - validateUser(user) from /utils/users.js 
    // - emailInUse(user.email) from /utils/users.js
    // - badRequest(response, message) from /utils/responseUtils.js
  }
};

module.exports = { handleRequest };