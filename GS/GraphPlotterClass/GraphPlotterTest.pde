
class GraphPlotter {
  // Width and height of the graph
  private final int width, height;
  // Center of the graph on the canvas.
  private final int centerX, centerY;
  // To rescale or not the graph when new value comes in.
  private boolean doRescale;
  
  GraphPlotter(int width, int height, int centerX, int centerY){
    if(height <= 0 || width <= 0){
      throw new IllegalArgumentException("Height and width must be bigger than 0"); 
    }
    this.height = height;
    this.width = width;
  
    this.centerX = centerX;
    this.centerY = centerY; 
    
    this.doRescale = false;
  }
  
  GraphPlotter(int width, int height, int centerX, int centerY, boolean rescale){
    if(height <= 0 || width <= 0){
      throw new IllegalArgumentException("Height and width must be bigger than 0"); 
    }
    this.height = height;
    this.width = width;
  
    this.centerX = centerX;
    this.centerY = centerY; 
    
    this.doRescale = rescale;
  }
  
  /**
   *@brief Draw the graph 
   *
   * @param isCentered : if the canvas is in the center of the canvas
   */
  public void drawGraph(){ //Make sure the canvas is at the origin !
    pushMatrix();
    translate(centerY - width/2, centerX - height/2);
    
    fill(255);
    stroke(0);
    strokeWeight(5);
    rect(0,0, width, height);
    
    
    popMatrix();
  }
  // ---------- SETTERS ---------- //
  // Change the rescale mode
  public void setRescaleMode(boolean val){
    this.doRescale = val;
  }
  
  // ---------- GETTERS --------- //
  // Getter for the rescale mode
  public boolean rescale(){
    return doRescale; 
  }
  
  // Getter for the width of the graph
  public int width(){
    return width;
  }
  
  // Getter for the height of the graph
  public int height(){
    return height;
  }
  
  //Getter for the x value of the center of the graph relative to the canvas
  public int centerX(){
     return centerX; 
  }
  
  //Getter for the y value of the center of the graph relative to the canvas
  public int centerY(){
     return centerY; 
  }
}
