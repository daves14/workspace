package de.simulator.client;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.dom.client.Style.Unit;
import com.google.gwt.user.client.ui.HTML;
import com.google.gwt.user.client.ui.RootLayoutPanel;
import com.google.gwt.user.client.ui.TabLayoutPanel;

public class Simulator implements EntryPoint {
	
	/*
	//Panels
    private VerticalPanel menu = new VerticalPanel();
    private VerticalPanel mainPanel = new VerticalPanel();
    private HorizontalPanel browserPanel = new HorizontalPanel();
    private HorizontalPanel configPanel = new HorizontalPanel();
    //Buttons
    private Button reloadButton = new Button("reload");
    private Button runButton = new Button("run");
    private Button pushButton = new Button("push");
    //Labels
    private Label title = new Label("Simulator");
    private Label channels = new Label("Kanäle");
    private Label test2 = new Label("Renderergebnis");
    private Label test3 = new Label("Bibliothek");
    private Label test4 = new Label("menu");
    */
	public void onModuleLoad() {
		// Create a three-item tab panel, with the tab area 1.5em tall.
	    TabLayoutPanel p = new TabLayoutPanel(1.5, Unit.EM);
	    p.add(new HTML("this"), "[this]");
	    p.add(new HTML("that"), "[that]");
	    p.add(new HTML("the other"), "[the other]");

	    // Attach the LayoutPanel to the RootLayoutPanel. The latter will listen for
	    // resize events on the window to ensure that its children are informed of
	    // possible size changes.
	    RootLayoutPanel rp = RootLayoutPanel.get();
	    rp.add(p);
		/*
		configPanel.add(test2);
		configPanel.add(test3);
		configPanel.addStyleName("configPanel");
		
		menu.add(test4);
		menu.addStyleName("menu");
		
		mainPanel.add(configPanel);
		mainPanel.add(channels);
		mainPanel.addStyleName("mainpanel");
		
		browserPanel.add(menu);
		browserPanel.add(mainPanel);
		browserPanel.addStyleName("browserpanel");
		
		RootPanel.get("entry").add(title);
		RootPanel.get("entry").add(browserPanel);
		*/
	}

}