void setup(){
  GraphPlotter plotter = new GraphPlotter(200, 100, 200, 200);  
  
  print(plotter.height());
  
  size(500, 600);
  background(255);
  
  plotter.drawGraph();
}

void draw(){
  
}
