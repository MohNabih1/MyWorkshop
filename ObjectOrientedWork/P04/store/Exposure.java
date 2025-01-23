package store;

public enum Exposure {
    SHADE {
        @Override
        public String toString() {
            return "shade";
        }
    },
    SUN {
        @Override
        public String toString() {
            return "full-sun";
        }
    },
    PARTSUN {
        @Override
        public String toString() {
            return "part-sun";
        }
    }
}

