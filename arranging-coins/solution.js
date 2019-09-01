/**
 * @param {number} n
 * @return {number}
 */
var arrangeCoins = function(n) {
    var b = 1;
    var a = 1;
    var c = -n * 2;
    var delta = -b + Math.sqrt(b * b - 4 * a * c);
    return Math.floor(delta / (2 * a));
};
