
package Display;
import java.awt.GridLayout;
import java.awt.LayoutManager;
import static Display.OptionMenu.f;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;




public class OptionMenu implements ActionListener{
    static JFrame f = new JFrame("Editor");
    static JPanel options = new JPanel();
    
    static JButton height_bttn = new JButton("Ceiling Height");
    static JTextField height_field = new JTextField();
    static JButton renderDistance_bttn = new JButton("Render Distance");
    static JTextField renderDistance_field = new JTextField();
    static JButton runSpeed_button = new JButton("Sprint Speed");
    static JTextField runSpeed_field = new JTextField();
    static JButton walkSpeed_button = new JButton("Walk Speed");
    static JTextField WalkSpeed_field = new JTextField();
    
    
    Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
   
    int width = (int) dim.getWidth();
    int height = (int) dim.getHeight();
    public static int ceilingHeight0=20,rednderDistance0=6000;
    public static double runSpeed0=0.9,walkSpeed0=0.5;    
    public static boolean test51 = false;
    Point p;

    public OptionMenu() {
        this.p = new Point(width,height);
    }
    public void optionsMenu(boolean tabTest){
   
        
        LayoutManager mgr = new GridLayout(0,2);
		
        f.add(options, BorderLayout.WEST);
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        f.setVisible(tabTest);
        f.toFront();
        boolean test51=true;
        f.setState(Frame.NORMAL);
        f.setEnabled(true);
        f.setSize(275, 500);
       
        options.setLayout(mgr );
        options.add(height_bttn);
        options.add(height_field);
        options.add(renderDistance_bttn);
        options.add(renderDistance_field);
        options.add(runSpeed_button);
        options.add(runSpeed_field);
        options.add(walkSpeed_button);
        options.add(WalkSpeed_field);
        
        
       height_bttn.addActionListener(this);
       renderDistance_bttn.addActionListener(this);
       runSpeed_button.addActionListener(this);
       walkSpeed_button.addActionListener(this);
       
      }
    public void listen(ActionEvent ae){
        if (ae.getSource().equals(height_bttn)){
            ceilingHeight0 = Integer.parseInt(height_field.getText());
        }
        if (ae.getSource().equals(renderDistance_bttn)){
            rednderDistance0 = Integer.parseInt(renderDistance_field.getText());
        }
        if (ae.getSource().equals(runSpeed_button)){
            runSpeed0 = Integer.parseInt(runSpeed_field.getText());
        }
        if (ae.getSource().equals(walkSpeed_button)){
            walkSpeed0 = Integer.parseInt(WalkSpeed_field.getText());
            
        }
        
            
                

            
            
        
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }
}
