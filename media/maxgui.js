// maxgui

function createAndAppend(tag, parent) {

   let thing = document.createElement(tag);
   if (parent)
      parent.appendChild(thing);
   else
      document.body.appendChild(thing);

   return thing;
}



let scriptCount = 0;
let scriptLoadCount = 0;
let mainScript = "";

function loadContent(contentDict) {

   // console.log(contentDict);

   mainScript = contentDict["script"];

   function loadMainScriptWhenReady() {

      // console.log(scriptCount, scriptLoadCount, mainScript);

      if (scriptCount != scriptLoadCount)
         return;

      if (!mainScript)
         return;

      let script = document.createElement('script');
      script.src = mainScript;
      script.type = 'text/javascript';

      document.body.appendChild(script);
   }

   let includes = contentDict["includes"];
   for (let index in includes) {

      let jsFile = includes[index];
      scriptCount++;

      let script = document.createElement('script');
      script.src = jsFile;
      script.type = 'text/javascript';

      script.addEventListener("load", () => {

         scriptLoadCount++;
         loadMainScriptWhenReady();
      });

      document.getElementsByTagName('head').item(0).appendChild(script);

   }

   let styles = contentDict["styles"];
   for (let index in styles) {

      let styleFile = styles[index];

      let link = document.createElement("link");
      link.setAttribute("rel", "stylesheet");
      link.setAttribute("type", "text/css");
      link.setAttribute("href", styleFile);

      document.getElementsByTagName('head').item(0).append(link);
   }

   loadMainScriptWhenReady();
}

// inline

function inlineHTML(parent, fileName) {

   let request = new XMLHttpRequest();
   request.open("GET", fileName, false);
   request.send(null);

   parent.innerHTML = request.responseText;
}

function move(element, x, y) {

   element.style["position"] = "absolute";
   element.style["left"] = x.toString() + "px";
   element.style["top"] = y.toString() + "px";
}

class BaseElement {

   constructor(tag, parent) {

      this.element = createAndAppend(tag, parent);
   }

   setAttribute(key, value) {
      return this.element.setAttribute(key, value);
   }

   setStyle(key, value) {
      this.element.style[key] = value;
   }

   appendChild(element) {
      this.element.appendChild(element);
   }

   move(x, y) {

      move(this.element, x, y);
   }

   inlineHTML(fileName) {

      inlineHTML(this.element, fileName);
   }
}

// inline

// header
function setupDocument(docWidth, leftMargin, topMargin) {

   if (docWidth)
      document.body.style["width"] = docWidth.toString() + "px";
   if (leftMargin)
      document.body.style.marginLeft = leftMargin.toString() + "px";
   if (topMargin)
      document.body.style.marginTop = topMargin.toString() + "px";
}
