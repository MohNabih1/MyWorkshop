public class Note {
    private Pitch pitch;
    private int octave;
    // This is the Note Constructor
    public Note() {
        this.pitch = null;
    }
    // This is the constructor with parameters
    public Note(Pitch pitch, int octave) {
        this.pitch = pitch;
	
 // Error handling statements
        if (octave < -5) {
            this.octave = -5;
        } else if (octave > 4) {
            this.octave = 4;
        } else {
            this.octave = octave;
       }
    }

// This is the public Method
 @Override
    public String toString() {
        if (pitch == null) {
            return " ";
      } else {
            return pitch.toString() + (octave != 0 ? octave : "");
      }
  }
}

