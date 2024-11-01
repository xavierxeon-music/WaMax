// a scale

if (typeof this.Scale === "undefined") {

   this.Scale = class {

      static startIndexMap = {
         "c": 0,
         "c#": 1,
         "d": 2,
         "d#": 3,
         "e": 4,
         "f": 5,
         "f#": 6,
         "g": 7,
         "g#": 8,
         "a": 9,
         "a#": 10,
         "b": 11
      }

      static predefinedList = [
         new Scale("c"),
         new Scale("g"),
         new Scale("d"),
         new Scale("a"),
         new Scale("e"),
         new Scale("b"),
         new Scale("f#"),
         new Scale("f"),
         new Scale("a#"),
         new Scale("d#"),
         new Scale("g#"),
         new Scale("c#")
      ];

      constructor(baseNote) {

         if (undefined == baseNote)
            this.baseNote = "c";
         else
            this.baseNote = baseNote;

         this.notes = [false, false, false, false, false, false, false, false, false, false, false, false];

         var startIndex = Scale.startIndexMap[baseNote];

         const majorNotes = [0, 2, 4, 5, 7, 9, 11];
         for (var offsetIndex in majorNotes) {
            var offset = majorNotes[offsetIndex];
            var index = (startIndex + offset) % 12;
            this.notes[index] = true;
         }
      }

      clear() {

         for (var index = 0; index < 12; index++)
            this.notes[index] = false;
      }

      setScale(text) {

         this.clear();

         if (13 != text.length) {
            post(text, "has wrong length", text.length, "\n");
            return;
         }

         if ("s" != text[0]) {
            post(text, "need to start with 's'", "\n");
            return;
         }

         for (var index = 0; index < 12; index++) {
            this.notes[index] = (1 == text[index + 1]);
         }
      }

      getScale() {

         var text = "s";

         for (var index = 0; index < 12; index++) {
            if (this.notes[index])
               text += "1";
            else
               text += "0";
         }
         return text;
      }

      setPredefined(baseNote, major) {

         this.clear();

         var startIndex = Scale.startIndexMap[baseNote];
         if (!startIndex)
            return;

         if (major) {
            const majorNotes = [0, 2, 4, 5, 7, 9, 11];
            for (var offset in majorNotes) {
               var index = (startIndex + majorNotes[offset]) % 12;
               this.notes[index] = true;
            }
         }
         else {
            const minorNotes = [0, 2, 4, 6, 7, 9, 11];
            for (var offset in minorNotes) {
               var index = (startIndex + minorNotes[offset]) % 12;
               this.notes[index] = true;
            }
         }
      }

      closestMatch(midiNote) {

         if (midiNote < 0)
            return 0;

         // midi 0 = C-2
         var scaleNote = midiNote % 12;
         var isC = (scaleNote == 0);
         var octaveC = (midiNote - scaleNote);

         // if note does not belong to scale
         if (!this.notes[scaleNote]) {

            // get distance to nearest up scale note
            var up = 1;
            while (!this.notes[(scaleNote + up) % 12]) {
               up++;

               if (up > 12) {
                  return midiNote; // scale has no notes!
               }
            }

            // get distance to nearest down scale note
            var down = 1;
            while (!this.notes[(scaleNote + 12 - down) % 12]) {
               down++;
            }

            if (up >= down) { // move down
               scaleNote = (scaleNote + 12 - down) % 12;
               if (isC)
                  scaleNote -= 12;

            }
            else { // move up
               scaleNote = (scaleNote + up) % 12;
            }
            //post("non scale note", scaleNote, up, down, "\n");
         }

         return octaveC + scaleNote;
      }
   }
}
