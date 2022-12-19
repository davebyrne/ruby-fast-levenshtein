RSpec.describe FastLevenshtein do
  it "has a version number" do
    expect(FastLevenshtein::VERSION).not_to be nil
  end

  it "calculates the distance" do
    a = "Sunday"
    b = "Saturday" 
    expect(FastLevenshtein.distance(a, b)).to eq(3)
  end

  it "doesn't matter the order of the parameters" do 
    a = "Sunday"
    b = "Saturday" 
    expect(FastLevenshtein.distance(a, b)).to eq(FastLevenshtein.distance(b, a))
  end
end
