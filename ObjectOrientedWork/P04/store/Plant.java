package store;

public class Plant extends Product {
    private Exposure exposure;

    public Plant(String species, double price, Exposure exposure) {
        super("Plant: " + species, price);
        this.exposure = exposure;
    }

    public Exposure getExposure() {
        return exposure;
    }
}

