document.getElementById("addButton").addEventListener("click", function(event) {
    event.preventDefault();
    const num1 = document.getElementById("addInput1").value;
    const num2 = document.getElementById("addInput2").value;
    console.log(binaryAddition(num1, num2));
});

function binaryAddition(num1, num2) {
    const binaryNum1 = parseInt(num1, 2);
    const binaryNum2 = parseInt(num2, 2);
    return binaryNum1 + binaryNum2;
}