// tabs

class TabContainer {

   constructor(tabbar, title) {

      // this.tabbar = tabbar;

      let id = btoa(title);

      this.tabbutton = createAndAppend("button", tabbar);
      this.tabbutton.setAttribute("class", "tabbutton");
      this.tabbutton.innerText = title;
      this.tabbutton.addEventListener("click", (clickEvent) => {
         tabbar.showTab(clickEvent.currentTarget, id);
      });

      this.tabcontent = createAndAppend("div");
      this.tabcontent.setAttribute("class", "tabcontent");
      this.tabcontent.setAttribute("id", id);

      this.tabcontent.style.display = "none";
   }

   appendChild(element) {
      this.tabcontent.appendChild(element);
   }

   setDefault() {
      this.tabbutton.className += " active";
      this.tabcontent.style.display = "grid";
   }

   setColor(hexColor) {
      this.tabbutton.style["color"] = hexColor;
   }
}

class TabBar extends BaseElement {

   constructor(defaultLayout, height) {

      super("div", document.body);
      this.layouttext = Div.extractLayout(defaultLayout);
      this.height = height;

      this.element.setAttribute("class", "tabbar");

      this.containerList = [];
   }

   appendChild(element) {
      this.element.appendChild(element);
   }

   setAttribute(key, value) {
      return this.element.setAttribute(key, value);
   }

   addTab(title, localLayout) {

      let container = new TabContainer(this, title);
      this.containerList.push(container);

      if (this.height)
         container.tabcontent.style["height"] = this.height.toString() + "px";

      if (localLayout) {
         let locallayouttext = Div.extractLayout(localLayout);
         container.tabcontent.style.gridTemplateColumns = locallayouttext;
      }
      else if (this.layouttext) {
         container.tabcontent.style.gridTemplateColumns = this.layouttext;
      }
      return container;
   }

   showTab(clickedButton, tabName) {

      for (let index in this.containerList) {

         let tabcontent = this.containerList[index].tabcontent;
         if (tabcontent.id == tabName)
            tabcontent.style.display = "grid";
         else
            tabcontent.style.display = "none";

         let tabbutton = this.containerList[index].tabbutton;
         tabbutton.className = tabbutton.className.replace(" active", "");
         if (tabbutton == clickedButton)
            tabbutton.className += " active";
      }
   }
}