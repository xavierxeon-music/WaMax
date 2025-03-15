// helpers


if (typeof this.print === "undefined") {

   this.print = function () {

      for (var index = 0; index < arguments.length; index++)
         post(arguments[index]);
      post("\n");
   }
}

if (typeof this.removeFromArray === "undefined") {

   this.removeFromArray = function (array, value) {

      for (var index = 0; index < array.length; index++) {

         if (array[index] !== value)
            continue;

         array.splice(index, 1);
         return;
      }
   }
}

if (typeof this.isHex === "undefined") {

   this.isHex = function (value) {

      for (var index = 0; index < 6; index += 1) {

         var test = value[index];
         switch (test) {
            case "0":
            case "1":
            case "2":
            case "3":
            case "4":
            case "5":
            case "6":
            case "7":
            case "8":
            case "9":
            case "a":
            case "b":
            case "c":
            case "d":
            case "e":
            case "f":
               continue;
            default:
               return false;
         }
      }

      return true;
   }
}

if (typeof this.makeHex === "undefined") {

   this.makeHex = function (number) {
      var hex = Math.round(number).toString(16);
      if (1 === hex.length)
         hex = "0" + hex;

      return hex;
   }
}

if (typeof this.createPseudoUuid === "undefined") {

   this.createPseudoUuid = function () {

      var s = [];
      var hexDigits = "0123456789abcdef";
      for (var i = 0; i < 36; i++) {
         s[i] = hexDigits.substr(Math.floor(Math.random() * 0x10), 1);
      }
      s[14] = "4";  // bits 12-15 of the time_hi_and_version field to 0010
      s[19] = hexDigits.substr((s[19] & 0x3) | 0x8, 1);  // bits 6-7 of the clock_seq_hi_and_reserved to 01
      s[8] = s[13] = s[18] = s[23] = "-";

      var uuid = s.join("");
      return uuid;
   }
}

