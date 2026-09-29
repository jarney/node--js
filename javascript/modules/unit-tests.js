

function UnitTests() {
}

UnitTests.prototype.run = function() {
    var console = document.getElementById("console");

    //    console.innerHTML = "someText";
    console.textContent = "Unit Test was run";
};

export { UnitTests };
