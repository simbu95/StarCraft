package stargui;

import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class StarThread extends Thread{
    
    BufferedReader in;
    PrintWriter out;
    JLabel M,W,S;
    JCheckBox C;
    JButton Supp;
    @Override
    public void run(){
        JFrame frame=new JFrame();
        
        frame.add(setup());
        frame.setSize(300, 200);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
        
        try {
            Socket s=new Socket("localhost",4998);
            
            in = new BufferedReader(new InputStreamReader(s.getInputStream()));
            out=new PrintWriter(s.getOutputStream(), true);
            C.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent ae) {
                    if(C.isSelected()){
                        out.println("1");
                        System.out.println("Send,1");
                    }
                    else{
                        out.println("0");
                        System.out.println("Send,0");
                    }
                }
            });
            Supp.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent ae) {
                    out.println("2");
                }
            });
            String inputLine;
                while ((inputLine = in.readLine()) != null) {
                    //System.out.println("recieved");
                    Process(inputLine);
                }
        } catch (IOException ex) {
            Logger.getLogger(StarThread.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    public void Process(String s){
        String[] stings=s.split(",",2);
        //System.out.println(s);
            switch(stings[0]){
                case "0":
                    M.setText(stings[1]);
                    break;
                case "1":
                    W.setText(stings[1]);
                    break;
                case "2":
                    S.setText(stings[1]);
                    break;
                default:
                    System.out.println("Error with string");
            }
    }
    
    public JPanel setup(){
        JPanel Con=new JPanel(new GridLayout(5,2));
        JLabel Minerals=new JLabel("Minerals");
        M=new JLabel();
        JLabel Workers=new JLabel("Workers");
        W=new JLabel();
        JLabel Soldiers=new JLabel("Soldiers");
        S=new JLabel();
        JLabel BuildW=new JLabel("Build Workers");
        C=new JCheckBox();
        Supp=new JButton("Supply Depot");
        Con.add(Minerals);
        Con.add(M);
        Con.add(Workers);
        Con.add(W);
        Con.add(Soldiers);
        Con.add(S);
        Con.add(BuildW);
        Con.add(C);
        Con.add(Supp);
        return Con;
    }
}
