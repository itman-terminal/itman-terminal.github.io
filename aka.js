var callback = function (args) {

console.log(args)
console.log('验证成功');
}; var expiredCallback = function (args) {

console.log(args)
console.log('验证过期');
}; var errorCallback = function (args) {

console.log(args)
console.log('验证失败');
};

var widgetId; var onloadCallback = function () {


widgetId = grecaptcha.render('robot', {
    'sitekey': '6LfMITAoAAAAAONNDa50_DtE6PLoHS7oykEB2_Qy', 
    'theme': 'light',
    'size': 'compact',
    'callback': callback, 
    'expired-callback': expiredCallback, 
    'error-callback': errorCallback 
});
};

function getResponseFromRecaptcha() {

var responseToken = grecaptcha.getResponse(widgetId);
if (responseToken.length == 0) {
    alert("验证失败");
} else {
    alert("验证通过");
    
}
};
