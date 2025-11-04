#include <core/pose/Pose.hh>
#include <core/import_pose/import_pose.hh>
#include <core/scoring/ScoreFunction.hh>
#include <core/scoring/ScoreFunctionFactory.hh>
#include <protocols/moves/MonteCarlo.hh>
#include <protocols/moves/PyMOLObserver.hh>
#include <core/pack/task/PackerTask.hh>
#include <core/pack/task/TaskFactory.hh>
#include <core/pack/pack_rotamers.hh>
#include <core/kinematics/MoveMap.hh>
#include <core/optimization/MinimizerOptions.hh>
#include <core/optimization/AtomTreeMinimizer.hh>
#include <numeric/random/random.hh>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "You didn’t provide a PDB file with the -in::file::s option" << std::endl;
        return 1;
    }

    core::pose::PoseOP mypose = core::import_pose::pose_from_file(argv[1]);
    core::scoring::ScoreFunctionOP sfxn = core::scoring::get_score_function();
    protocols::moves::MonteCarlo mc(*mypose, *sfxn, 2.0); // Initialize MonteCarlo object with temperature 2.0
    protocols::moves::PyMOLObserverOP the_observer = protocols::moves::AddPyMOLObserver(*mypose, true, 0);
    // Send the starting pose to PyMOL before any changes are made
    the_observer->pymol().apply(*mypose);

    // Declare objects for minimization outside the loop
    core::kinematics::MoveMap mm;
    mm.set_bb(true);
    mm.set_chi(true);

    core::optimization::MinimizerOptions min_opts("lbfgs_armijo_atol", 0.01, true);
    core::optimization::AtomTreeMinimizer atm;

    core::pose::Pose copy_pose;

    // 2. Begin loop
    for (int i = 0; i < 100; ++i) { // Perform 100 iterations
        // 3. Perturb the phi/psi values for your Pose
        core::Size randres = numeric::random::rg().random_range(1, mypose->total_residue()); // Random residue index
        core::Real pert1 = numeric::random::rg().gaussian() * 10.0; // Random perturbation for phi (scaled by 10 degrees)
        core::Real pert2 = numeric::random::rg().gaussian() * 10.0; // Random perturbation for psi (scaled by 10 degrees)

        core::Real orig_phi = mypose->phi(randres); // Get original phi angle
        core::Real orig_psi = mypose->psi(randres); // Get original psi angle

        mypose->set_phi(randres, orig_phi + pert1); // Set new phi angle
        mypose->set_psi(randres, orig_psi + pert2); // Set new psi angle

        // Packing
        core::pack::task::PackerTaskOP repack_task =
            core::pack::task::TaskFactory::create_packer_task(*mypose);
        repack_task->restrict_to_repacking();
        core::pack::pack_rotamers(*mypose, *sfxn, repack_task);

        // Minimization
        copy_pose = *mypose;
        atm.run(copy_pose, mm, *sfxn, min_opts);
        *mypose = copy_pose;

        // 4. Call MonteCarlo object’s boltzmann method, passing it your Pose
        mc.boltzmann(*mypose);
    }

    return 0;
}